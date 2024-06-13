#!/bin/bash

if [[ "$(id -u)" -ne 0 ]]; then 
	echo "Program is not executed by root"
	exit 1
fi

while read user; do
	TOTAL_RSS=0
	PS_COUNT=0
	MAX_RSS=-1
	MAX_PID_RSS=-1

	while read upid urrs; do
		PSCOUNT=$(( PS_COUNT + 1 ))
		TOTAL_RSS=$(( TOTAL_RSS + urss ))

		MAX_RSS="${urss}"
		MAX_PID_RSS="${upid}"
	done < <(ps -u "${user}" -o pid=,rss= | sort -n -k 2) 

	if [[ "${PS_COUNT}" -eq 0 ]]; then
		continue
	fi

	AVG_RSS=$((TOTAL_RSS / PS_COUNT ))

	echo "${user} ${PS_COUNT} ${TOTAL_RSS}"
	if [[ "${MAX_RSS}" -gt $(( AVG_RSS * 2 )) ]]; then
		echo "Terminating process "${MAX_PID_RSS}""
		kill -s TERM "${MAX_PID_RSS}"
		sleep 2
		kill -s KILL "${MAX_PID_RSS}"
	fi
done < <(ps -e -o user= | sort | uniq)
