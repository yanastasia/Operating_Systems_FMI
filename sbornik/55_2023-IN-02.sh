#!/bin/bash

if [[ ! "$#" -eq 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo ""${DIR}" not a directory"
	exit 1
fi

files=$(mktemp)
hlinks=$(mktemp)
shasums=$(mktemp)

while read fileName; do
	if [[ "$(stat -c '%h' "${fileName}")" -lt 2 ]]; then
		echo "$(sha256sum "${fileName}")" >> "${files}"
	else
		echo "$(sha256sum "${fileName}")" >> "${hlinks}"
	fi

	echo "$(sha256sum "${fileName}")" >> "${shasums}"
done < <(find "${DIR}" -type f -printf '%f\n')

candidates=$(mktemp)

while read checksum; do 
	if [[ "$(grep "${checksum}" "${files}" | wc -l)" ]] && [[ "$(grep "${checksum}" "${hlinks}"| wc -l ]]; then 
		echo "$(grep "${checksum}" "${files}" | awk  '{print $2}')" >> "${candidates}"
		echo "$(grep "${checksum}" "${hlinks}" | awk '{print $2}' | head -n 1)" >> "${candidates}"
	elif [[ "$(grep "${checksum}" "${files}" | wc -l)" ]]; then
		echo "$(grep "${checksum}" "${files}" | awk '{print $2}' | head -n -1)" >> "${candidates}"
	else
		echo "$(grep "${checksum}" "${hlinks}" | awk '{print $2}' | head -n 1)" >> "${candidates}"
	fi
done < <(cat "${shasums}" | awk '{print $1}' | uniq)

cat "${candidates}"

rm -- "${files}"
rm -- "${hlinks}"
rm -- "${shasums}"
rm -- "${candidates}"

