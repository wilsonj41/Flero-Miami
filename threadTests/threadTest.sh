#!/bin/bash
xargs -I % -P 5 curl -w "@curl-format.txt" -s "http://localhost:8080/hello.elf" < <(printf '%s\n' {1..5})

#xargs -I % -P 5 
