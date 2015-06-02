#!/usr/bin/env bash

progname=$0

usage(){
	echo "Usage: $progname [input entries file] [output header prefix]" >&2
}

if test $# -ne 2; then
	echo "error: invalid the number of the arguments" >&2
	usage
	exit 1
elif ! test -f "$1"; then
	echo "error: file $1 not found" >&2
	usage
	exit 1
fi

b=$(basename "$1" .txt)
bc=$(echo "$b" | tr -s '[a-z]' '[A-Z]')
ig=_ENTRIES_${bc}_H_
n=$(cat "$1" | grep -v '^[ 	]*$' | grep -v '^[ 	]*#' | wc -l)
file="$2/entries_$b.h"

(
echo "#ifndef $ig"
echo "#define $ig"
echo
echo "#include <stdint.h>"
echo "#include <avr/pgmspace.h>"
echo
echo "static const int32_t entries_$b[$n][3] PROGMEM = \\"
prog/entries_to_c.py <"$1"
echo
echo "#endif /* $ig */"
) >"$file"
