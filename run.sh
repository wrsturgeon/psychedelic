#!/bin/bash

if [ -z "${3}" ]; then echo "Usage: `${0} <executable> <src-image-path> <dst-image-path> [debug]`"; exit 1; fi

set -ex

EXECUTABLE=bin/${1}
IMAGE_W=$(identify ${2} | cut -d' ' -f3 | cut -dx -f1)
IMAGE_H=$(identify ${2} | cut -d' ' -f3 | cut -dx -f2)

clang -o gauss_kernel_compute src/gauss_kernel_compute.c -lm
./gauss_kernel_compute 0.7071067812
rm gauss_kernel_compute

git submodule update --init --recursive

# Compile
if [ ! -f ${EXECUTABLE} ]; then
  echo "Compiling..."

  mkdir -p bin
  if [ "${4}" = "debug" ]; then
    ARGS='-DINLINE= -g -O0'
  else
    ARGS='-DINLINE=EIGEN_ALWAYS_INLINE -Ofast -march=native -funit-at-a-time -mllvm -polly -mllvm -polly-vectorizer=stripmine'
  fi
  clang++ src/main.cpp -o ${EXECUTABLE} ${ARGS} -DIMAGE_H=${IMAGE_H} -DIMAGE_W=${IMAGE_W} -pedantic -Wall -Wextra -Werror -Wno-c++17-extensions -Wno-c11-extensions -Wno-c99-extensions -Ieigen $(pkg-config --cflags --libs opencv4)
  echo "Compiled successfully!"
fi

if [ "${4}" = "debug" ]; then
  lldb bin/${1} --batch -o run\ ${2}\ ${3} -k thread\ backtrace -k q\ 1
else
  bin/${1} ${2} ${3}
fi
