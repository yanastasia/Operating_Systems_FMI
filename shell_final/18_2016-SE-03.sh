#!/bin/bash

if [[ "$(id -u)" -ne 0 ]]; then
	echo "Script not run by root"
	exit 1
fi

while read user home; do
	if [[ -z "${home}" ]] || [[ "$(stat -c "%A" "${home}" | cut -c 3)" = '-' ]]; then
		echo "${user}"
	fi
done < <(cat /etc/passwd | awk -F ':' '{print $1, $6}')
