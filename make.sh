#!/usr/bin/env bash

SCRIPT_PATH="$(readlink -f "${BASH_SOURCE[0]}")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"
cd "$SCRIPT_DIR" || exit 1

rm -rf build

mkdir -p build && cd build

# export CFLAGS="-Ofast"
export MUSL_CROSS_ROOT="/opt/raiden"
#export CMAKE_CXX_FLAGS

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Ofast -Wall -Wextra -Wpedantic -g -fwhole-program -s -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion" ..

cmake --build . --config Release -- -j$(nproc) || exit 1

cd ..

cp build/bin/APP releases/main

echo ">>"
echo "  >>  `du -sh ./releases/*`"
echo "  >>  `file ./releases/*`"
echo "<<"

time ./releases/*
