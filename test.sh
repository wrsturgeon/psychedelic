#!/bin/bash
set -ex

rm -rf bin

IMAGE_I='alias_bricks.jpg'
IMAGE_O='gauss.png'

# Check internet connection
set +x
wget -q --spider -t 1 --timeout=1 http://google.com
set -x
if [ ${?} -eq 0 ]; then
  mkdir -p images
  wget https://upload.wikimedia.org/wikipedia/commons/3/31/Moire_pattern_of_bricks.jpg -O images/${IMAGE_I}.new && \
  mv images/${IMAGE_I}.new images/${IMAGE_I}
  rm -f images/${IMAGE_I}.new
else
  if [ ! -f images/${IMAGE_I} ]; then
    echo "Can't connect to internet; need a file to work with at images/${IMAGE_I}"
    exit 1
  fi
fi

./run.sh test_executable images/${IMAGE_I} images/${IMAGE_O} ${1} # ${1} to debug...if you're into that kind of thing

if [ "$(identify images/${IMAGE_I} | cut -d' ' -f3)" != "622x756" ] ; then
  echo "$(identify images/${IMAGE_I} | cut -d' ' -f3) =/= 622x756"
  exit 1
fi

if [ "$(identify images/${IMAGE_O} | cut -d' ' -f3)" != "310x377" ] ; then
  echo "$(identify images/${IMAGE_O} | cut -d' ' -f3) =/= 310x377 = (622-2)/2 x (756-2)/2"
  exit 1
fi
