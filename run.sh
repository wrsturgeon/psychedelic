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

  DEPENDENCIES="opencv4" # Eigen included as a submodule

  # Find dependencies
  COMPILER_FLAGS=
  for dep in ${DEPENDENCIES}; do
    if INCLUDES_AND_LIBS=$(pkg-config --cflags --libs ${dep} 2>/dev/null); then
      echo "Found ${dep}"
      COMPILER_FLAGS="${COMPILER_FLAGS} ${INCLUDES_AND_LIBS}"
    else
      echo "No includes or libraries found for ${dep}"
    fi
  done

  mkdir -p bin
  if [ "${2}" = "debug" ]; then
    ARGS='-DINLINE= -g -O0'
  else
    ARGS='-DINLINE=EIGEN_ALWAYS_INLINE -Ofast -march=native -funit-at-a-time -mllvm -polly -mllvm -polly-vectorizer=stripmine'
  fi
  clang++ src/main.cpp -o ${EXECUTABLE} ${ARGS} -pedantic -Wall -Wextra -Werror -Wno-c++17-extensions -Wno-c11-extensions -Wno-c99-extensions -Ieigen ${COMPILER_FLAGS}
  echo "Compiled successfully!"
fi

if [ "${2}" = "debug" ]; then
  echo -e "run\nq\n" > lldb.script
  lldb bin/${1} -s lldb.script
  rm lldb.script
else
  bin/${1}
fi
