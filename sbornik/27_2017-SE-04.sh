#!/bin/bash

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo "${DIR} does not exist"
	exit 1
fi

if [[ ! -r "${DIR}" ]]; then
	echo "${DIR} not readable"
	exit 1
fi

if [[ "$#" -eq 2 ]]; then
	FILE="${2}"
fi

if [[ ! -f "${FILE}" ]]; then
	touch "${FILE}"
fi

count=0

while read link; do
	file="$(readlink -f "${link}")"
	if [[ $(file "${link}" | grep 'broken' | wc -l) ]]; then
		count=$(( count + 1 ))
	fi

	if [[ "$#" -eq 2 ]]; then
		echo ""$(basename "${link}")" -> "${file}"" >> "${FILE}"
	else
		echo ""($basename "${link}")" -> "${file}""
	fi

done < <(find "${DIR}" -type l)

echo "Broken symlinks: "${count}""


