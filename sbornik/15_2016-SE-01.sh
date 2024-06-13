#!/bin/bash

[[ "$#" -eq 1 ]] || {echo "Invalid number of arguments"; exit 1;}

[[ -d "${1}" ]] || {echo "Argument not a directory"; exit 1;}

dir="${1}"

while read link; do
	if [[ $(file "${link}" | grep 'broken') ]]; then 
		echo "${link}"
	fi
done < <(find "${dir}" -type l)
