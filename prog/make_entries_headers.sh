#!/usr/bin/env bash

for f in example.txt; do
	prog/entries_to_header.sh entries/"$f" prog/arduino/src/
done
