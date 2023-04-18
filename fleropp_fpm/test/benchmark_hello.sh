#!/usr/bin/env bash

for i in {1..30}; do
    echo -n "${1}," >> hello_timing.dat
    curl -w "@curl-format.txt" -o /dev/null -s "${2}" >> hello_timing.dat
done
