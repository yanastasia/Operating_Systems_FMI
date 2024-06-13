#!/bin/bash

if [[ "$#" -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

DIR="${1}"

if [[ ! -d "${DIR}" ]]; then
	echo "Argument not a directory"
	exit 1
fi

file $(find "${DIR}" -type l) | grep 'broken' 
