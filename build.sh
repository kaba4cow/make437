#!/bin/bash

set -e

mkdir -p build
gcc \
  -Wall \
  -Wextra \
  -Iinclude/stb \
  -o build/make437 \
  src/make437.c \
  -lm