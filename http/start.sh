#!/bin/bash

ROOT=$(pwd)
LIB=$ROOT/lib/lib
#.
export LD_LIBRARY_PATH=${LIB}; ./httpd 9999
