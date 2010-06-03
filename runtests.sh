#!/bin/bash

################################################################
# Make the parser, if it is not already built
pwd=`pwd`
PARSER="$pwd/parser"
if [ $PARSER ]
then
	make
fi

passed=0
failed=0

################################################################
# And start performing the tests
dir="tests"
cd $dir

### Queries that have "hql" extension should be parsed successfully
echo
for file in `ls *.hql`; do
	echo "Test $file"
	# If exit code is 0
	if $PARSER < $file > /dev/null 2>/dev/null
		then 
			echo "... ok"
			passed=$((passed+1))
		else 
			echo "... failed ..."
			failed=$((failed+1))
	fi
done

echo 
### Queries that have "hql.fail" extension should fail gracefully
for file in `ls *.hql.fail`; do
	echo "Test $file"
	# If exit code is 0
	if $PARSER < $file > /dev/null 2>/dev/null
		then 
			echo "... failed ..."
			failed=$((failed+1))
		else 
			echo "... ok"
			passed=$((passed+1))
	fi
done


### Report results
echo
echo
echo "Passed tests: $passed"
echo "Failed tests: $failed"
