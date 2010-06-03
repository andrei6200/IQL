#!/bin/bash

dir="tests"

passed=0
failed=0


for file in `ls $dir`; do
	if ./parser < $dir/$file > /dev/null 2>/dev/null
		then 
			echo "Test $file ok"
			passed=$((passed+1))
		else 
			echo "Test $file failed ..."
			failed=$((failed+1))
	fi
done

echo
echo
echo "Passed tests: $passed"
echo "Failed tests: $failed"
