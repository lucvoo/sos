#!/bin/sh

# $1 = depfile
# $2 = target
# $3 = rule
# $4 = cmdfile

scripts/basic/fixdep "$1" "$2" "$3" > "$4".tmp && \
rm -f "$1" && \
mv -f "$4".tmp "$4"
