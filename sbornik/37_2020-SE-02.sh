#!/bin/bash

if [[ "$#" -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

FILE="${1}"

if [[ ! -r  "${FILE}" ]]; then 
	echo "File not readable"
	exit 1
fi

sites=$(mktemp)
clients=$(mktemp)

while read line; do
	user_address="$(echo "${line}" | cut -d ' ' -f 1 )"
	host="$(echo "${line}" | cut -d ' ' -f 2)"


	code="$(echo "${line}" | rev | cut -d '/' -f 1 | rev | cut -d ' ' -f 2)"
	
	echo "${host}" >> "${sites}"

	if [[ "${code}" -gt 302 ]]; then 
		echo "${user_address}" >> "${clients}"
	fi
done < "${file}"

while read site; do
	echo "test: ${site}"
	TOTAL_HTTP_2=$(cat "${file}" | grep "${site}" | grep "HTTP/2.0" | wc -l)
	TOTAL_NON_HTTP_2=$(cat "${file}" | grep "${site}" | grep -v "HTTP/2.0" | wc -l)

	echo "${site} HTTP/2.0: ${TOTAL_HTTP_2} non-HTTP/2.0: ${TOTAL_NON_HTTP_2}"
done < <(cat "${sites}" | sort | uniq c | sort -nr | head -n 5)

while read user; do
	echo "${user}"
done < <(cat "${clients}" | sort | uniq -c | sort -rn | head -n 5)

rm -- "${sites}"
rm -- "${clients}"
