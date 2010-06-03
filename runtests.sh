#!/bin/bash

dir="tests"

passed=0
failed=0


for file in `ls $dir`; do
	filename="$dir/$file"
	echo "Test $filename"
	if ./parser < $filename > /dev/null 2>/dev/null
		then 
			echo "... ok"
			passed=$((passed+1))
		else 
			echo "... failed ..."
			failed=$((failed+1))
	fi
done

echo
echo
echo "Passed tests: $passed"
echo "Failed tests: $failed"
