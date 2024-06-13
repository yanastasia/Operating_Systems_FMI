#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"
arr="${2}"

if [[ ! -d "${DIR}" ]]; then
	echo "Invalid directory"
	exit 1
fi

if [[ ! -r "${DIR}" ]]; then
	echo "Directory not readable"
	exit 1
fi

if [[ ! -z "${arr}" ]]; then
	echo "array empty"
	exit 1
fi

temp=$(mktemp)

find "${DIR}" -maxdepth 1 -type f -printf '%f\n' | grep -E "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-"${arr}"">"${temp}"

searched="$(cat "${temp}" | cut -d '-' -f 2 | sort -t '.' -n -k 1,1 -k 2,2 -k 3,3 | tail -n 1)"

fgrep "${searched}" "${temp}"

rm -- "${temp}"


