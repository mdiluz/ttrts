#! /bin/bash
# Use to generate the ttrts maps

ttrtsgen=$1

test ! -e maps && mkdir maps # Make maps directory if needed
if [ ! -e maps ]; then
    exit 1
fi

cd maps

$ttrtsgen