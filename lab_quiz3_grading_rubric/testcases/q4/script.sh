#!/bin/bash

g++ alloc.cpp test_alloc.c -o test_alloc
./test_alloc > out1.txt
g++ alloc.cpp test_alloc_hidden.c -o test_alloc_hidden
./test_alloc_hidden > out2.txt

diff out1.txt sample_output.txt 2>/dev/null 1>&2
echo $?
diff out2.txt hidden_output.txt 2>/dev/null 1>&2
echo $?
