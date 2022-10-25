#!/bin/sh

cd $(dirname $0)
unset PATH_INFO REQUEST_METHOD QUERY_STRING

failed=0
for test_file in *
do
	[ "$test_file" = "$(basename $0)" -o "$test_file" = "shrt.db" ] && continue
	printf '%s ... ' $test_file
	if sh $test_file >/dev/null 2>&1
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
