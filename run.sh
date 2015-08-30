#!/bin/sh
python2 ./scripts/text2bin.py $1 > ${1%.txt}.bin && ./fifth < ${1%.txt}.bin
