#!/bin/bash

while read file; do
	#hashSuffix=".hash"
	md5sum "${file}" | awk '{print $1 $2}' > "$(basename "${file}")".hash""
	cat "$(basename "${file}")".hash""
done < <(find "${1}" -type f -regex ".*\.hash$")
