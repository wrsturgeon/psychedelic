#!/bin/bash

if [ ! -z "${1}" ] && [ "${1}" != "debug" ]; then echo "Usage: `${0} [debug]`"; exit 1; fi

set -ex

rm -rf bin

IMAGE_I='alias_bricks.jpg'
IMAGE_O='gauss.png'

mkdir -p images
wget https://upload.wikimedia.org/wikipedia/commons/3/31/Moire_pattern_of_bricks.jpg -O images/${IMAGE_I}

./run.sh test_executable images/${IMAGE_I} images/${IMAGE_O}

# if [ ! -f images/${IMAGE_O} ] ; then exit 1; fi
