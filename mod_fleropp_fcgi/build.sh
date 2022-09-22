#!/usr/bin/env bash

docker run -v "$(pwd):$(pwd)" -w "$(pwd)" -i -t "build_container" /bin/bash -c "make"