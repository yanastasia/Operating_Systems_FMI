#!/bin/bash

if [[ "$#" -ne 2 ]]; then 
	echo "Invalid number of arguments"
	exit 1
fi

PAR1="${1}"
PAR2="${2}"

mkdir a
mkdir b
mkdir c

while read lines fileName; do
	if [[ "${lines}" -lt "${PAR1}" ]]; then 
		mv "${fileName}" a/
	elif [[ "${lines}" -gt "${PAR2}" ]]; then
		mv "${fileName}" c/
	else
		mv "${fileName}" b/
	fi
done < <(wc $(find . -type f) -l | awk '{print $1,$2}')
