#!/bin/bash

last_user=0
last_file=0
last_time=-1

while read user home; do 
	if [[ ! -d "${home}" ]] || [[ ! -r "${home}" ]]; then
		continue
	fi

	cur_file=$(find "${home}" -type f -printf "%T %f\n" | sort -n -t ' ' -k1 | tail -n 1)

	if [[ "$(echo "${cur_file}" | awk '{print $1}')" -gt "${last_time}" ]]; then 
		last_time="$(echo "${cur_file}" | awk '{print $1}')"
		last_user="${user}"
		last_file="$(echo "${cur_file}" | awk '{rpint $2}')"
	fi

done < <(cat etc/passwd | awk -F ':' '{print $1, $6}')

echo "The user is "${last_user}" with file "${last_file}""
