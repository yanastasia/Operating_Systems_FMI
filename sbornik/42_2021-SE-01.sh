#!/bin/bash

if [ "$#" -gt 0 ]; then
	echo "Too many arguments"
	exit 1
fi

if [ "$(id -un)" != "oracle"  ] && [ "$(id -un)" != "grid"]; then
	echo "Program can't be executed"
	exit 1
fi

if [ -z "${ORACLE_HOME}" ]; then
	echo "ORACLE_HOME variable empty"
	exit 1
fi

if [ ! -x "${ORACLE_HOME}/bin/ardci" ]; then
	echo "No executable file ardci"
	exit 1
fi

output=$(exec ${ORACLE_HOME/bin/ardci} exec="show homes")

if [ $output == "No ADR homes are set" ]; then
	echo "No ADR homes are set"
	exit 0;
fi

while read line; do
	path="${ORACLE_HOME}/${line}"
	size=$(du -sm "${line}" | awk '{print $1}')
	echo "${size} ${path}"
done < <(echo "${output}" | tail -n +2)



