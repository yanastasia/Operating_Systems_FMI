#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [[ ! -f "${FILE}" ]]; then
	echo ""${FILE}" not a file"
	exit 1
fi

if [[ ! -r "${FILE}" ]]; then
	echo "file not readable"
	exit 1
fi

if [[ ! -d "${DIR}" ]]; then
	echo ""${DIR}" not a directory"
	exit 1
fi

if [[ ! -r "${DIR}" ]]; then
	echo "directory not readable"
	exit 1
fi

while read fileName; do
	while read word; do
		censored_word=$(echo "${word}" | sed 's/./\*/g')
		sed -i "s/\b"${word}"\b/"${censored_word}"/g" "${fileName}"
	done < <(cat "${FILE}")
done < <(find "${DIR}" -type f | grep "\.txt$")
