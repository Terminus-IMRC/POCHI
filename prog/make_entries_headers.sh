#!/usr/bin/env bash

for f in example.txt WhiteGlint.txt mission3.txt; do
	b=$(basename "$f" .txt)
	prog/entries_to_header.py "$b" <entries/"$f" >prog/arduino/src/entries_"$b".h
done
