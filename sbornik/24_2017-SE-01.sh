#!/bin/bash

if [[ "$#" -eq 2 ]]; then
	NUM="${2}"
elif [[ "$#" -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"
[[ -d "${DIR}" ]] || {echo "${DIR} not a directpry"; exit 1;}
[[ -r "${DIR}" ]] || {echo "${DIR} not readable"; exit 1;}

if [[ "$#" -eq 2 ]]; then 
	while read file; do
		num_hardlinks=$(find "${DIR}" -samefile "${file}" | wc -l)
		if [[ "${num_hardlinks}" -ge "${num}" ]]; then
			echo "${file}"
		fi
	done < <(find "${DIR}" -type f)
else
	while read link; do
		if [[ $(file "${link}" | grep 'broken') ]]; then
			echo "${link}"
		fi

	done < <(find "${DIR}" -type l)
fi





