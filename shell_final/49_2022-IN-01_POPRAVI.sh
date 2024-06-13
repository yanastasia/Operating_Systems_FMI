#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR1="${1}"
DIR2="${2}"

if [[ ! -d "${DIR1}" ]] || [[ ! -d "${DIR2}" ]]; then
	echo "Directories not available"
	exit 1
fi

while read file; do
	dest_path=""${DIR2}""${file#DIR1}""

	mkdir -p "$(dirname "${dest_path}")"

	cp "${file}" "${dest_path}"
done < <(find "${DIR1}" -type f | grep -v  '.*.swp$') 
