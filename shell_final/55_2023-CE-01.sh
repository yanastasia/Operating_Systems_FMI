#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

FILE="${1}"
TYPE="${2}"

if ! [[ -f "${FILE}" && -r "${FILE}" ]]; then
	echo ""${FILE}" is not a file or readable"
	exit 1
fi

stars=$(mktemp)

while read line; do
	if [[ "$(echo "${line}" | awk -F ',' '{print $5}')" = "${TYPE}" ]]; then
		echo "$(echo "${line}" | awk -F ',' '{print $1 "," $4 "," $7}')" >> "${stars}"
	fi
done < <(cat "${FILE}" | grep "${TYPE}")

#cat "${stars}"

constallation="$(cat "${stars}" | awk -F ',' '{print $2}' | uniq -c | head -n 1 | awk '{print $2}')"

stars_in_cons=$(mktemp)

while read s_cons; do
	if [[ "$(echo "${s_cons}" | awk -F ',' '{print $2}')" = "${constallation}" ]]; then
		echo "$(echo "${s_cons}" | awk -F ',' '{print $1 "," $3}')" >> "${stars_in_cons}"
	fi
done < <(cat "${stars}")

cat "${stars_in_cons}" | sort | head -n 1 | awk -F ',' '{print $1}'

#cat "${stars}" | awk -F ',' '{print $2}' | uniq -c
rm -- "${stars}"
rm -- "${stars_in_cons}"
