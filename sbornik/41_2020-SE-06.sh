#!/bin/bash

if [ "$#" -ne 3 ]; then
	echo "Arguments not equal to 3"
	exit 1
fi

file="${1}"
key="${2}"
value="${3}"

if [ ! -f "${file}" ]; then
	echo "Invalid file\n"
	exit 1
fi

if [[ "${key}" =~ ^[[:allnum]_]+$ ]]; then
	echo "Invalid key\n"
	exit 1
fi

if  [[ "${value}" =~ ^[[:allnum]_]+$ ]]; then 
	echo "Invalid value\n"
	exit 1
fi

temp=$(mktemp)

while read line; do
	if [[ $(echo "${line}" | grep "${key}" | wc -l) -eq 0 ]]; then 
		echo "${line}" >> "${temp}"
		continue
	fi

	val=$(echo "${line}" | cut -d '#' -f 1 | cut -d '=' -f 2- | sed 's/ //')

	if [[ "${value}" =~ "${val}" ]]; then 
		echo "${line}" >> "${temp}"
		continue
	fi

	oldmessage="#""${line}"" # edited at $(date) by $(whoami)"
	newmessage="${key} = ${value} # added at $(date) by $(whoami)"

	echo "${oldmessage}" >> "${temp}"
	echo "${newmessage}" >> "${temp}"

done < <(cat "${file}")

if [[ $(cat "${file}" | cut -d '=' -f 1 | grep "${key}") -eq 0 ]]; then
	echo "${key} = ${value} # added at $(date) by $(whoami)" >> "${temp}"
fi

cat "${temp}" > "${file}"

rm "${temp}"

