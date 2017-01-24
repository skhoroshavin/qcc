#!/bin/sh
$1/scripts/pre_test.sh $1
find CMakeFiles/qcc.dir/ -name *.gcda -type f -delete || true
rm -rvf coverage* > /dev/null
