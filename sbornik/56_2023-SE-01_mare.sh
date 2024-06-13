#!/bin/bash

if [[ ! "${#}" -eq 2 ]]; then
	echo "invalid number of arguments"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [[ ! -f "${FILE}" ]]; then
	echo ""${FILE}" is not a file"
	exit 1
fi

if [[ ! -r "${FILE}" ]]; then
	echo "File is not readable"
	exit 1
fi

if [[ ! -d "${DIR}" ]]; then
	echo "DIR isn't a dir"
	exit 1
fi

while read fileName; do
	while read badWord; do
		censored_word=$(echo "${badWord}" | sed 's/./\*/g')
		sed -i 's/\b${word}\b/"${censored_word}"/g' "${fileName}"
	done < <(cat "${FILE}")
done < <(find "${DIR}" -type f | grep "\.txt$")

