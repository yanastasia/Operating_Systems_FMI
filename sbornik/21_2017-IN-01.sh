#!/bin/bash

[[ "$#" -eq 3 ]] || {echo "Invalid number of arguments"; exit 1;}

FILE="${1}"
KEY1="${2}"
KEY2="${3}"

[[ -f "${FILE}" ]] || {echo "${FILE} not file"; exit 1;}

[[ -r "${FILE}" ]] || {echo "${FILE} not readable"; exit 1;}

[[ -n "${KEY1}" ]] || {echo "${KEY1} is with length 0"; exit 1;}
[[ -n "${KEY2}" ]] || {echo "${KEY2} is with length 0"; exit 1;}

VALUE1=$(cat "${FILE}" | grep "^${KEY1}" | awk -F '=' '{print $2}' | tr ' ' '\n')

VALUE2=$(cat "${FILE}" | grep "^${KEY2}" | awk -f '=' '{print $2}' | tr ' ' '\n')

result=""

while read value; do 
	if [[ $(echo "${VALUE1}" | grep "${value}" | wc -l) -eq 0 ]]; then
		result=$(echo ""${result}" "${value}"")
	fi

done < <(echo "${VALUE2}")

sed -i -E "s/(${VALUE2}=).*/\1${result}/g" "${file}"
