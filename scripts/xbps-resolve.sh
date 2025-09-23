#!/bin/sh

echo "resolving libs, in case of multiple choose one"

for arg in "$@"; do
    xlocate $arg.pc | fzf -1 | cut -d' ' -f1
done
