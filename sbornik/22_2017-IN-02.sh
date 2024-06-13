#!/bin/bash

[[ "$#" -eq 1 ]] || {echo "Invalid number of arguments"; exit 1;}

[[ "$(id -u)" -eq 0 ]] || {exit1;}

FOO="${1}"

PS=$(mktemp)

ps -e -o user=,pid=,etimes= | sort -t ' ' -k 1 > "${PS}"

FOO_PS_COUNT=$(cat "${PS}" | cut -d ' ' -f 1 | grep "${FOO}" | wc -l)

echo "a)"

while read user; do
	user_count=$(cat "${PS}" | cut -d ' ' -f 1 | grep "${user}" | wc -l)

	if [[ "${user_count}" -gt "${FOO_PS_COUNT}" ]]; then
		echo "${user}"
	fi

done < <(cat "${PS}" | cut -d ' ' -f 1 | sort -u )

echo "\nb)"

AVG=$(cat "${PS}" | awk '(total += $3) END {print int(total/NR)}')

echo "${AVG}"

echo "\nc)"

while read PID ETIMES; do 
	if [[ "${ETIMES}" -gt $(echo "${AVG} * 2" | bc -l) ]]; then
		kill -s TERM "${PID}"
		sleep 2
		kill -s KILL "${PID}"		
	fi
done < <(cat "${PS}" | grep "${FOO}" | cut -d ' ' -f 2-)

rm -- "${PS}"
