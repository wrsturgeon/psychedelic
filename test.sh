#!/bin/bash

set -ex

mkdir -p images
wget https://upload.wikimedia.org/wikipedia/commons/3/31/Moire_pattern_of_bricks.jpg -o images/alias_bricks.jpg

./run.sh test_executable debug
