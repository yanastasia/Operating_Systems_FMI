#!/bin/bash

if [[ "$#" -eq 3 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

file1="${1}"
file2="${2}"
dir="${3}"

if [[ ! -f "${file1}" ]]; then
	echo "Invalid file in first argument"
	exit 1
fi

if [[ ! "${file2}" ]]; then
	echo "Invalid file in second argument"
	exit 1
fi

if [[ ! -d "${dir}" ]]; then
	echo "Invalid directory in third argument"
	exit 1
fi

if [[ ! -r "${dir}" ]]; then
	echo "Directory not readable"
	exit 1
fi

tempValid=$(mktemp)

while read fileName; do
	error="$(cat fileName | egrep -nv "^{.*};$" | egrep -nv "^#")"
	if [[ ! -z "${error}" ]]; then
		echo "Error in ${fileName}: "
	else
		echo "${fileName}" >> "${tempValid}"
		continue
	fi

	echo "Line ${error}"
	
done < <(find "${dir}" -type f | egrep "\.cfg$")

while read line; do
	cat "${line}" >> "${file2}"

	name="$(basename "${line}" | cut -d '.' -f 1)"
	if [[ $(cat "{$file1}" | fgrep "${name}" | wc -l) -eq 0 ]]; then 
		generated_pw=$(pwgen,16,1)
		echo "${name}:${generated_pw}" >> "${file1}"
		echo "${name} ${generated_pw}"
	fi

done < "${tempValid}"

rm -- "${tempValid}"
