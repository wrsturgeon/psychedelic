#!/bin/bash

set -ex

rm -rf bin

mkdir -p images
wget https://upload.wikimedia.org/wikipedia/commons/3/31/Moire_pattern_of_bricks.jpg -O images/alias_bricks.jpg

./run.sh test_executable images/alias_bricks.jpg ${1} # ${1} to debug...if you're into that kind of thing

if [ "$(identify ${2} | cut -d' ' -f3)" != "310x377" ] ; then
  echo "$(identify ${2} | cut -d' ' -f3) =/= 310x377 = (622-2)/2 x (756-2)/2"
  exit 1
fi
