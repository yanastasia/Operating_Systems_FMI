#!/bin/bash

[[ "$#" -eq 1 ]] || {echo "Invalid number of arguments"; exit 1;}

FILE="${1}"

[[ -f "${FILE}" ]] || {echo "${FILE} not a file"; exit 1}

[[ -r "${FILE}" ]] || {echo "${FILE} not readable"; exit 1}

temp=$(mktemp)
count=1
while read line; do

	echo "${count}.$(echo "${line}")" >> "${temp}"
	count=$(( count + 1 ))

done < <(cat "${FILE}" | cut -d '-' -f 2- )

cat "${temp}" | sort -t ' ' -k 2

rm -- "${temp}"


