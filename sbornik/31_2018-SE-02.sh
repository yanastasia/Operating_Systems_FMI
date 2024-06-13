#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [[ $(ls "${DIR}" | wc -l) -gt 0 ]]; then
	echo "The directory is not empty"
	exit 1
fi

count=1 

touch "${DIR}/dict.txt"

while read line; do
	name="$(echo "${line}" | cut -d ':' -f 1 | cut -d '(' -f 1 | tr -s ' ')"
	
	echo "${name}";"${count}" >> "${DIR}/dict.txt"

	touch "${DIR}/${number}.txt"
	echo "${line}" | cut -d ':' -f 2 >> "${DIR}/${number}.txt"

	count=$(( count + 1 ))
done < <(cat "${file}")
