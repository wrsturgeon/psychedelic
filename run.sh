#!/bin/bash

if [ -z "${1}" ]; then echo "Call ${0} like so: ${0} <executable-name> [debug]"; exit 1; fi

set -ex

EXECUTABLE=bin/${1}

clang -o gauss_kernel_compute src/gauss_kernel_compute.c -lm
./gauss_kernel_compute 0.7071067812
rm gauss_kernel_compute

git submodule update --init --recursive

# Compile
if [ ! -f ${EXECUTABLE} ]; then
  echo "Compiling..."

  mkdir -p bin
  if [ "${2}" = "debug" ]; then
    ARGS='-DINLINE= -g -O0'
  else
    ARGS='-DINLINE=EIGEN_ALWAYS_INLINE -Ofast -march=native -funit-at-a-time -mllvm -polly -mllvm -polly-vectorizer=stripmine'
  fi
  clang++ src/main.cpp -o ${EXECUTABLE} ${ARGS} -pedantic -Wall -Wextra -Werror -Wno-c++17-extensions -Wno-c11-extensions -Wno-c99-extensions -Ieigen $(pkg-config --cflags --libs opencv4)
  echo "Compiled successfully!"
fi

if [ "${2}" = "debug" ]; then
  echo -e "run\nq\n" > lldb.script
  lldb bin/${1} -s lldb.script
  rm lldb.script
else
  bin/${1}
fi
