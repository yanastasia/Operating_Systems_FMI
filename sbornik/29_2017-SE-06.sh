#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
	echo "Script is not ran by root"
	exit 1
fi

TOTAL_ROOT_RSS=$(ps -e -u "root" -o rss= | awk '{i+=$1} END {print i}')

while read user home; do

	if [[ "${user}" -eq 0 ]]; then 
		continue
	fi

	if [[ ! -d "${home}" ]] || [[ "$(stat -c "%u" "${home}")" != "${user}" ]] ||  [[ "$(stat -c '%A' "${home}"| cut -c 3) = '-' ]]; then
		TOTAL_USER_RRS="$(ps -e -u "${user}" -o rss= | awk '{i+=$1} END {print i}')"

		if [[ "${TOTAL_USER_RSS}" -gt "${TOTAL_ROOT_RSS}" ]]; then
			killall -u "${user}" -KILL
		fi
	fi

done < <(cut -d ':' -f 3,6 /etc/passwd)

