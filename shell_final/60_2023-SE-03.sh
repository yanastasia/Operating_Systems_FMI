#!/bin/bash

if [[ "$#" -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo ""${DIR}" not a directory"
	exit 1
fi

fileCountHalf=$(( $(find "${DIR}" -type f | wc -l) / 2 ))
candidates=$(mktemp)

while read fileName; do
	cat "${fileName}" | grep -o '\b[a-z]*\b' | sort | uniq -c | awk '{ if ($1 >= 3) {print $1, $2}}' >> "${candidates}"
done < <(find "${DIR}" -type f)

stopwords=$(mktemp)
last_word=""
while read word; do
	if [[ "${last_word}" = "${word}" ]]; then 
		continue
	fi

	if [[ "$(grep -c "${word}" "${candidates}")" -ge "${fileCountHalf}" ]]; then
		"${last_word}"="${word}"
		echo "$(grep "${word}" "${candidated}" | awk '{i+=$1} END {print $i}')" "${word}" >> "${stopwords}"
	fi

done < <(cat "${candidates}" | sort -k 2 | awk '{print $2}')

cat "${stopwords}" | sort -nk 1 | head -n 10 | awk '{print $2}'

rm -- "${candidates}"
rm -- "${stopwords}"
