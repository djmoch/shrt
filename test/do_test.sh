#!/bin/sh

cd $(dirname $0)
unset PATH_INFO REQUEST_METHOD QUERY_STRING

for test_file in *
do
	failed=0
	[ "$test_file" = "$(basename $0)" -o "$test_file" = "shrt.db" ] && continue
	printf '%s ... ' $test_file
	if sh $test_file
	then
		echo PASSED
	else
		echo FAILED
		failed=1
	fi
done

if [ $failed -ne 0 ]
then
	exit 1
fi
