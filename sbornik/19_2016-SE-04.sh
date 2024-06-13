#!/bin/bash

[[ "$#" -eq 2 ]] || {echo "Invalid number of parameters"; exit 1;}

FILE1="${1}"
FILE2="${2}"

[[ -f "${FILE1}" ]] || {echo "${FILE1} not file"; exit 1;}
[[ -f "${FILE2}" ]] || {echo "${FILE2} not file"; exit 1;}

[[ -r "${FILE1}" ]] || {echo "${FILE1} not readable"; exit 1;}
[[ -r "${FILE2}" ]] || {echo "${FILE2} not readable"; exit 1;}

if [[ $(cat "${FILE1}" | grep "${FILE1}" | wc -l) -ge $(cat "${FILE2}" | grep "${FILE2}" | wc -l) ]]; then
	FILE="${FILE1}"
else
	FILE="${FILE2}"
fi

cat "${FILE}" | cut -d ' ' -f 4- | sort > ""${FILE}.songs"
