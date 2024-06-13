#!/bin/bash

if [[ "$#" -ne 1 ]] || [[ "$#" -ne 2]]; then 
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo "${DIR} not a directory"
	exit 1
fi

if [[ "$#" -eq 2 ]]; then
	FILE="${2}"
fi

if [[ ! -d "${FILE}" ]]; then 
	echo "${FILE} not a regular file"
	exit 1
fi

count=0

while read link; do
	file="$(readlink -f "${link}")"
	if [[ $(file "${link}" | grep 'broken' | wc -l) ]]; then
		count=$(( ${count} + 1))
	fi

	if [[ "$#" -eq 2 ]]; then 
		echo ""${link}" -> "${file}"" >> "${FILE}"
	else
		echo ""${link}" -> "${file}""
	fi
done < <(find "${DIR}" -type l)

echo "Broken symlinks: "${count}""
