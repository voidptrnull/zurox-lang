#!/bin/sh

rm build -rf
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C build -j$(nproc)