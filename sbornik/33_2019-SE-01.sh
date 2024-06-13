#!/bin/bash

temp=$(mktemp)

maxnum=-1

while read line; do
	if [[ "$(echo "${line}" | egrep "^[-+]?[0-9]+$" | wc -l)" -gt 0 ]]; then
		echo "${line}" >> "${temp}"
		if [[ "${line}" -gt "${maxnum}" ]]; then
			maxnum="${line}"
		fi
		if [[ $(( line * -1 )) -gt "${maxnum}" ]]; then
			maxnum=$(( line * -1 ))
		fi
	fi 
done

while read num; do
	if [[ "${num}" -eq "${maxnum}" ]]; then
		echo "${num}"
	elif [[ $(( num * -1 )) -eq "${maxnum}" ]]; then
		echo "${num}"
	fi

done < <(cat "${temp}")

rm -- "${temp}"
