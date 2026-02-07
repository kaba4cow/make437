#!/bin/bash

set -e

mkdir -p build
gcc \
  -Wall \
  -Wextra \
  -o build/make437 \
  src/make437.c