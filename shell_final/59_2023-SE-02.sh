#!/bin/bash

if [[ "$#" -lt 3 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

limit=$1
command=${@:2}

count=0
total_time=0

while true; do
	if [[ $(echo "${total_time} < ${limit}" | bc -l) ]]; then 
		break
	fi
	start_time=$(date +%s.%N)
	${command}
	end_time=$(date +%s.%N)
	execution_time=$(echo "${end_time} - ${start_time}" | bc -l)

	total_time=$(echo "${total_time} + ${execution_time}" | bc -l)
	count=$(( "${count}"+1 ))
done

wait

average_time=$(echo "scale=2; ${total_time} / ${count}" | bc -l)
echo ""${count}" times for "${total_time}"."
echo "Average time "${average_time}""
