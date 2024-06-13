#!/bin/bash

if [[ "$(id -u)" -ne 0 ]]; then
	echo "Program not run by root"
	exit 1
fi

if [[ "$#" -ne 1 ]]; then 
	echo "Ivalid number of arguments"
	exit 1
fi

BORDER="${1}"

while read user; do
	P_SUM=0
	PID=0
	while read p_pid p_rss; do
		P_SUM=$(( "${P_SUM}" + "${p_rss}" ))
		PID="${p_pid}"
	done < <(ps -u "${user}" -o pid=,rss= | sort -nk2)
	if [[ "${P_SUM}" -gt "${BORDER}" ]]; then 
		kill -s TERM "${PID}"
		sleep 3
		kill -s KILL "${PID}"
	fi
done < <(ps -e -o user= | sort | uniq)
