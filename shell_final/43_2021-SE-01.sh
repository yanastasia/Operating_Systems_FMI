#!/bin/bash

if [[ "$#" -ne 0 ]]; then
	echo "Invalid number of arguments"
	exit 0
fi

if [[ "$(id -un)" != "oracle" ]] && [[ "$(id -un)" != "grid" ]]; then
	echo "You don't have the right"
	exit 1
fi

if [[ ! -z "${ORACLE_HOME}" ]]; then
	echo "ORACLE_HOME variable empty"
	exit 1
fi

if [[ ! -x "${ORACLE_HOME}/bin/ardci" ]]; then
	echo "No executable file"
	exit 1
fi

output=$(exec ${ORACLE_HOME}/bin/ardci exec="show homes")

if [[ "${output}" = "No ADR homes are set" ]]; then
	echo "${output}"
	exit 0
fi

while read line; do
	path="${ORACLE_HOME}/${line}"
	size=$(du -sm "${line}" | awk '{print $1}')
	echo "${size} ${path}

done < <(echo "${output}" | tail -n +2)

