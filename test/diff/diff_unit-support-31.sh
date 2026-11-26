#!/usr/bin/env bash

file=unit-support-31

source test/diff/diff_source.sh

# x*y-1 and x (generators 1 and 3) generate the unit ideal, no other
# subset of two or fewer generators does
$(pwd)/msolve -f input_files/$file.ms --unit-support test/diff/$file.1.res
if [ $? -gt 0 ]; then
    print_exit 1
fi

diff test/diff/$file.1.res output_files/$file.res
if [ $? -gt 0 ]; then
    print_exit 2
fi

rm test/diff/$file.1.res

# a system that does not generate the unit ideal is refused with status 2
$(pwd)/msolve -f input_files/cyclic5-31.ms --unit-support test/diff/$file.2.res
if [ $? -ne 2 ]; then
    print_exit 3
fi
rm -f test/diff/$file.2.res

normal_exit
