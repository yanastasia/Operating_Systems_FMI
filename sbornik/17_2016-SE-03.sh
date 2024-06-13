#!/bin/bash

[[ "$(id -u)" -eq 0 ]] || {echo "Script not run by root"; exit 1;}

while read user home; do
	if [[ -z "${home}" ]] || [[ $(stat -c "%A" "${home}" | cut -c 3) == '-' ]]; then
		echo "${user}" 
	fi
done < <(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ')
