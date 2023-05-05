#!/usr/bin/env bash

n_primes=(1 10 100 1000 10000 100000 1000000)

for n in ${n_primes[@]}; do
    echo "#time_total,time_namelookup,time_connect,time_appconnect,time_pretransfer,time_redirect,time_starttransfer" >> "${1}_${n}_primes_timing.dat"
    for i in {1..15}; do
        curl -w "@curl-format.txt" -o /dev/null -s "${2}?n=${n}" >> "${1}_${n}_primes_timing.dat"
    done
done
