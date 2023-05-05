#!/usr/bin/env bash

n_primes=(1 10 100 1000 10000 100000 1000000)

for n in ${n_primes[@]}; do
    echo "#walltime" >> "${1}_${n}_primes_timing.dat"
    for i in {1..15}; do
	TIMEFORMAT='%3R'; time (${2} ${n}) 2>> "${1}_${n}_primes_timing.dat"
    done
done
