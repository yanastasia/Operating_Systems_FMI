#!/bin/bash

SRC="${1}"
DIR="${2}"

if [[ ! -d "${SRC}" ]]; then
	echo "Firectory "${SRC}" does not exist"
	exit 1
fi

mkdir -p "${DIR}/images"
mkdir -p "${DIR}/by-date"

while read fileName; do
	title=$(echo "${fileName}" | cut -d '.' -f 1 | sed 's/([^)]*)//g' | tr -s ' ')

	if [[ $(echo "${fileName}" | grep '(' | wc -l) -gt 0 ]]; then
		album=$(echo "{fileName}" | rev | cut -d '(' -f 1 | cut -d ')' -f 2 | rev | tr -s ' ')
	fi

	date=$(stat "${fileName}" --printf "%y\n" | cut -d ' ' -f 1)
	hash=$(sha256sum "${fileName}" | cut -d ' ' -f 1)

	dest="${DIR}/images/${fileName}"
	cp "${fileName}" "${dest}"

	if [[ ! -d "${DIR}/by-date/${date}/by-album/${album}/by-title/" ]]; then
		mkdir -p "${DIR}/by-date/${date}/by-album/${album}/by-title/"
	fi

	ln -s "${dest}" "${DIR}/by-date/${date}/by-album/${album}/by-title/${title}.jpg"

	if [[ ! -d "${DIR}/by-date/${date}/by-title/" ]]; then
		mkdir -p "${DIR}/by-date/${date}/by-title/" 
	fi

	ln -s "${dest}" "${DIR}/by-date/${date}/by-title/"

	if [[ ! -d "${DIR}/by-album/${album}/by-date/${date}/by-title/" ]]; then
		mkdir -p "${DIR}/by-album/${album}/by-date/${date}/by-title/"
	fi

	ln -s "${dest}" "${DIR}/by-album/${album}/by-date/${date}/by-title/${title}.jpg"

	if [[ ! -d "${DIR}/by-album/${album}/by-title/" ]]; then
		mkdir -p "${DIR}/by-album/${album}/by-title/"
	fi

	ln -s "${dest}" "${DIR}/by-album/${album}/by-title/${title}.jpg"

	if [[ ! -d "${DIR}/by-title/" ]]; then
		mkdir -p "${DIR}/by-title/"
	fi

	ln -s "${dest}" "${DIR}/by-title/${title}.jpg"

done < <(find "${SRC}" -type f -printf '%f\n' | egrep "\.jpg$")
