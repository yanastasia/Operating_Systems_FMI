#!/bin/bash

if [[ "$#" -lt 1 ]]; then 
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo "${DIR} does not exist"
	exit 1
fi

if [[ ! -r "${DIR}" ]]; then
	echo "${DIR} not readable"
	exit 1
fi

all_archives=$(find "${DIR}" -mindepth 1 -maxdepth 1 -type f -name "*_report-*.tgz$" -printf "%p %M@\n")
last_execution=$(stat -c "%X" ${0})
result_archives=$(echo "${all_archives}" | awk -v var=${last_execution} '{if($2>=var) print $1}')

while read fileName; do
	name=$(echo "${fileName}" | sed -E 's/(.*)_/\1/')
	timestamp=$(echo "${filename}" | sed -E 's/report-(.*)[.]tgz/\1/')

	while read line; do
		bn=$(basename "${line}")
		if [[ "${bn}" == "meow.txt" ]] && [[ -f "${bn}" ]]; then
			tar -cf "${fileName}" "${line}"
			dir=$(dirname "${file}")
			mv "${DIR}/${line}" "/extracted/${name}_${timestamp}.txt"
			break
		fi
	done < <(tar -tf "${file}" | egrep "meow.txt")
done < <(echo "${result_archives}")
