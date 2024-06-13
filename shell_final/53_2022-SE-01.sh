#!/bin/bash

if [[ "$#" -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

CONFIG="${1}"

if ! [[ -f "${CONFIG}" ]] && [[ -r "${CONFIG}" ]]; then
	echo ""${CONFIG}" not a file or not readable"
	exit 1
fi

FILE="/proc/acpi/wakeup"

DEVICES=$(cat "${FILE}" | tail -n +1 | awk '{print $1, $3}')

while read device status; do
	while read d s; do
		if [[ "${d}" = '#' ]] || [[ "${s}" = '#' ]]; then
			continue;
		fi

		if [[ "${d}" = "${device}" ]] && ! [[ "${s}" = "${status}" ]]; then
			echo "${device}" >> "${FILE}"
		fi
	done < <(cat "${CONFIG}" | awk '{print $1, $2}')
done < <(cat "${DEVICES}" | awk '{print $1, $2}')
