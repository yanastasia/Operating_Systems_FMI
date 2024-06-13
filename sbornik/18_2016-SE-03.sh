#!/bin/bash

[[ "$#" -eq 2 ]] || {echo "Invalid number of arguments"; exit 1;}

num1="${1}"
num2="${2}"

mkdir a
mkdir b
mkdir c

while read fileName; do
	lines=$(cat "${fileName}" | wc -l)
	if [[ "${lines}" -lt "${num1}" ]]; then 
		mv "${fileName}" a/
	elif [[ "${lines}" -gt "${num1}" ]] && [[ "${lines}" -lt "${num2}" ]]; then
		mv "${fileName}" b/
	else
		mv "${fileName}" c/
	fi
done < <(find . -type f)
