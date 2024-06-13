#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

FILE1="${1}"
FILE2="${2}"

if [[ ! -f "${FILE1}" ]]; then
	echo "Invalid file"
	exit 1
fi

touch "${FILE2}"

while read line; do
	ID=$(echo "${line}" | cut -d ',' -f 1)
	REST=$(echo "${line}" | cut -d ',' -f 2-)

	if [[ $(cat "${FILE2}" | fgrep ","${REST}"" | wc -l) -gt 0 ]]; then
		continue
	fi

	echo "${line}" >> "${FILE2}"
done < <(cat "${FILE1}" | sort -n -t ',' -k 1)
