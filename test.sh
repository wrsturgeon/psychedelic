#!/bin/bash

set -ex

rm -rf bin

mkdir -p images
wget https://upload.wikimedia.org/wikipedia/commons/3/31/Moire_pattern_of_bricks.jpg -O images/alias_bricks.jpg

./run.sh test_executable images/alias_bricks.jpg

if [ ! -f images/poop.png ] ; then exit 1; fi
