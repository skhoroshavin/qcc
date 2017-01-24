#!/bin/sh
$1/scripts/post_test.sh $1
lcov --capture --directory CMakeFiles/qcc.dir/ --output-file coverage.info
genhtml coverage.info --output-directory coverage
