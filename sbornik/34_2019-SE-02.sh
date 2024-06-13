#!/bin/bash

args="${@}"

N=10
if [[ "$(echo "${args}" | grep '\-n' | wc -l)" -gt 0 ]]; then
	N="$(echo "${args}" | cut -d ' ' -f 2)"
fi

temp=$(mktemp)

while read name; do
	IDF=$(echo "${name}" | cut -d '.' -f 1)
	while read line; do
		time=$(echo "${line}" | cut -d ' ' -f 1,2)
		interval=$(echo "${line}" | cut -d ' ' -f 3)
		data=$(echo "${line}" | cut -d ' ' -f 4-)

		echo "${time}" "${interval}" "${IDF}" "${interval}" "${data}" >> "${temp}"
	done < <(cat "${name}" | sort -n -k 1,1 -k 2,2 | head -n "${N}") 
done < <(echo "${args}" | tr ' ' '\n')

while read line; do
	echo "${line}"
done < <(cat "${temp}" | sort -n -k 1,1 -k 2,2)

rm -- "${temp}"
