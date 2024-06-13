#!/bin/bash

if [[ "$#" -ne 3 ]]; then
	echo "Arguments not equal to 3"
	exit 1
fi

FILE="${1}"
KEY="${2}"
VALUE="${3}" 

if [[ ! -f "${FILE}" ]]; then 
	echo "Invalid key\n"
	exit 1
fi

if [[ "${KEY}" =~ ^[[:allnum]_]+$ ]]; then 
	echo "Invalid key\n"
	exit 1
fi

if [[ "${VALUE}" =~ ^[[:allnum]_]+$ ]]; then
	echo "Invalid value\n"
	exit 1
fi

temp=$(mktemp)


