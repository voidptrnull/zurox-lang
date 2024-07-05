#!/bin/sh

# NOTE: ONLY TO BE RUN FOR GITHUB ACTIONS

# VERSION
LLVM_VERSION="17"
cd $(pwd)/scripts/
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sh llvm.sh "$LLVM_VERSION"