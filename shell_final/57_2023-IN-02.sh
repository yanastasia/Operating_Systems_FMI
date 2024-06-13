#!/bin/bash

if [[ ! "$#" -eq 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo ""${DIR}" not a directory"
	exit 1
fi

files=$(mktemp)
hlinks=$(mktemp)
shasums=$(mktemp)

while read fileName; do
	#separating out only the hardlinks and files
	if [[ "$(stat -c '%h' "${fileName}")" -lt 2 ]]; then
		#stat -c '%h' <fileName> returns how many hardlinks it has => if it returns >1 
		#that means the file has hardlinks writing the shasum of file in files temporary file
		#output of the sha256sum function is <shasum of file> <name of file>
		echo "$(sha256sum "${fileName}")" >> "${files}"
	else
		echo "$(sha256sum "${fileName}")" >> "${hlinks}"
	fi
	
	echo "$(sha256sum "${fileName}")" >> "${shasums}"
done < <(find "${DIR}" -type f -printf '%f\n')

candidates=$(mktemp)

while read checksum; do
	if [[ "$(grep "{checksum}" "${files}" | wc -l)" ]] && [[ "$(grep "${checksum}" "${hlinks}" | wc -l)" ]]; then
		#checking if the checksum is written both in regular files and hardlinks => 
		#we are writing all filenames in the candidates file and one filename from the hardlinks file 
		#to the cadidates file
		echo "$(grep "${checksum}" "${files}" | awk '{print $2}')" >> "${candidates}"
		echo "$(grep "${checsum}" "${hlinkd}" | awk '{print $2}' | head -n 1)" >> "${candidates}"
	elif [[ "$(grep "${checksum}" "${files}" | wc -l)" ]]; then
		#checking if the shasum(checksum) is present in the files file and writing evey filename except one into the candidates file
		echo "$(grep "${checksum}" "${files}" | awk '{print $2}' | head -n -1)" >> "${candidates}"
	else
		#if the checksum is only in the hardlinks file then we write one of the filenames into the candidates file
		echo "$(grep "${checksum}" "${hlinks}" | awk '{print $2}' | head -n 1)" >> "${candidates}"
	fi
#we are only interested in the shasums which are the first column of the shasum file and 
#only the uniq ones in order not to duplicate outputs
done < <(cat "${shasums}" | awk '{print $1}' | uniq)

cat ${candidates}"

rm -- "${files}"
rm -- "${hlinks}"
rm -- "${shasums}"
rm -- "${candidates}"
