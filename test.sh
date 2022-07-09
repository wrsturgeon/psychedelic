#!/bin/bash

if [ ! -z "${1}" ] && [ "${1}" != "debug" ]; then echo "Usage: `${0} [debug]`"; exit 1; fi

set -ex

rm -rf bin

IMAGE_I='alias_bricks.jpg'
IMAGE_O='gauss.png'

mkdir -p images
wget https://upload.wikimedia.org/wikipedia/commons/3/31/Moire_pattern_of_bricks.jpg -O images/${IMAGE_I}

./run.sh test_executable images/${IMAGE_I} images/${IMAGE_O} ${1} # ${1} to debug...if you're into that kind of thing

if [ "$(identify images/${IMAGE_I} | cut -d' ' -f3)" != "622x756" ] ; then
  echo "$(identify images/${IMAGE_I} | cut -d' ' -f3) =/= 622x756"
  exit 1
fi

# if [ "$(identify images/${IMAGE_O} | cut -d' ' -f3)" != "310x377" ] ; then
#   echo "$(identify images/${IMAGE_O} | cut -d' ' -f3) =/= 310x377 = (622-2)/2 x (756-2)/2"
#   exit 1
# fi
