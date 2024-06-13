#!/bin/bash

if [ -z "$1" ]; then
	echo "Provide argument for hours"
	exit 1
fi

if ![ $1=~^[0-9]+$ ]; then
	echo "Argument is not a positive integer"
	exit 1
fi

if [ $1 -lt 2 ]; then
	echo "Argument is less than 2"
	exit 1
fi

if [ "$(id -un)" != "oracle"  ] && [ "$(id -un)" != "grid"]; then
	echo "Program can't be executed"
	exit 1
fi

DIAG_DEST="/u01/app/$(id -un)"
ORACLE_HOME=
