#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

if [ "$1" == "64" ]; then
    echo "Building and running in 64-bit mode..."
    make all && make run64
else
    echo "Building and running in 32-bit mode..."
    make all && make run
fi
