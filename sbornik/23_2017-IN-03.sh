#!/bin/bash

last_user=0
last_file=0
last_time=-1

while read user home; do
	[[ -d "${home}" ]] || continue
	[[ -r "${home}" ]] || continue

	cur_file=$(find "${home}" -type f -print "%T@ %f\n" | sort -n -t ' ' -k 1 | tail -n 1)"

	if [[ "$(echo "${cur_file}" | cut -d ' ' -f 1)" -gt "${last_time}" ]]; then
		last_time="$(echo "${cur_file}" | cut -d ' ' -f 1)"
		last_user="${user}"
		last_file="$(echo "${cur_file}" | cut -d ' ' -f 2)"
	fi

done < <(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ')

echo "The user is "${last_user}" with file "${last_file}""
