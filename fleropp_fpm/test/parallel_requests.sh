#!/usr/bin/env bash

# A quick and dirty shell script to perform parallel connection tests on the FPM
# Usage: ./parallel_requests.sh <n_replicates> <n_parallel_requests> <sites_list>
# Example: ./parallel_requests.sh 5 20 websites.txt

replicates=$1
n_parallel=$2
sites_list=$3

for i in $(seq 1 $replicates); do
    curl -v --parallel --parallel-immediate --parallel-max $n_parallel --config $sites_list
done