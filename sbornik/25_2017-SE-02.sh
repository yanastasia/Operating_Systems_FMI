#!/bin/bash

if [[ "$#" -ne 3 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

SRC="${1}"
DST="${2}"
ABC="${3}"

if [[ ! -d "${SRC}" ]]; then
	echo "${SRC} is not a directory"
	exit 1
fi

[[ -d "${DST}" ]] || {echo "${DST} is not a directory"; exit 1;}

dst_content=$(find "${DST}" -type f | wc -l)

[[ "${dst_content}" -gt 0 ]] || {echo "${DST} is not empty"; echo 1;}

[[ "$(id -u)" -eq 0 ]] || {echo "Script not run by root"; exit 1;}

while read file; do
	real_dir_name="$(echo "$(file}" | sed -E "s/"${SRC}"\///")"

	mkdir -p "${DST}/"$(dirname "${real_dir_name}")"

	mv "${file}" "${DST}"/"${real_dir_name}"

done < <(find "${SRC}" -type f -name "*${ABC}*")
