#!/bin/sh

rm build -rf
cmake -B build -G Ninja
ninja -C build -j$(nproc)