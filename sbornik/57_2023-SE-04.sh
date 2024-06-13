#!/bin/bash

if [[ "$#" -ne 1 ]]; then 
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

shasums=$(mktemp)

disk_used=$(du -s --bytes "${DIR}" | awk '{print $1}')

while read file; do
	echo "$(sha256sum "${file}")" >> "${shasums}"
done < <(find "${DIR}" -type f -printf '%f\n')

cur_sum=""
cur_file=""


while read checksum fileName; do
	if [[ "${checksum}" = "${cur_sum}" ]]; then
		ln -sf "${cur_file}" "${fileName}"
		continue
	fi

	"${cur_sum}"="${checksum}"
	"${cur_file}"="${fileName}"
done< <(cat "${shasums}" | sort -k 1,1 | awk 'print $1 $2')

deduplicated="$(cat "${shasums}" | awk '{print $1}' | uniq -d | wc -l)"

cur_space="$(du -s --bytes "${DIR}" | awk '{print $1}')"

space_freed=$(( "${disk_used}" - "${cur_space}" ))
echo "Number of groups deduplicated: "${deduplicated}""
echo "Amount of space freed: "${space_freed}""

rm -- "${shasums}"
