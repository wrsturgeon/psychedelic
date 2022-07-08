#!/bin/bash

set -e

if [ -z "${1}" ]; then echo "Call ${0} like so: ${0} <executable-name> [debug]"; exit 1; fi

EXECUTABLE=bin/${1}

clang -o gauss_kernel_compute src/gauss_kernel_compute.c -lm
./gauss_kernel_compute 0.7071067812
rm gauss_kernel_compute

git submodule update --init --recursive

# Compile
if [ ! -f ${EXECUTABLE} ]; then
  echo "Compiling..."

  DEPENDENCIES="opencv llvm" # Eigen included as a submodule

  # Find dependencies
  COMPILER_FLAGS=
  # brew upgrade
  # brew cleanup
  for dep in ${DEPENDENCIES}; do
    # (brew upgrade ${dep} && brew cleanup ${dep}) || brew install ${dep}
    set +e; PC_PATH=$(find /usr -name ${dep}*.pc -print -quit 2>/dev/null); set -e
    if [ ! -z "${PC_PATH}" ]; then
      COMPILER_FLAGS="${COMPILER_FLAGS} $(pkg-config --cflags --libs ${PC_PATH})"
    else
      echo "No includes or libraries found for ${dep}"
    fi
  done

  mkdir -p bin
  if [ "${2}" = "debug" ]; then
    ARGS="-g -O0 -DEXECNAME=${1}"
  else
    ARGS="-Ofast -march=native -funit-at-a-time -mllvm -polly -mllvm -polly-vectorizer=stripmine"
  fi
  clang++ src/main.cpp -o ${EXECUTABLE} ${ARGS} -pedantic -Wall -Wextra -Werror -Wno-c++17-extensions -Wno-c11-extensions -Wno-c99-extensions -Ieigen ${COMPILER_FLAGS}
  echo "Compiled successfully!"
fi

if [ "${2}" = "debug" ]; then
  echo -e "run\nthread backtrace\nq\n" > lldb_cmd
  lldb ${EXECUTABLE} -s lldb_cmd
  rm lldb_cmd
else
  bin/${1}
fi
