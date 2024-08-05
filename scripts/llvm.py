#!/usr/bin/env python

import os
import subprocess
import multiprocessing

LLVM_TARGETS = "X86;ARM;PowerPC"
LLVM_VERSION = "18.1.8"

# Define the base directory (assumes the script is in a subdirectory of the project root)
dir = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir))

# Define paths for LLVM source and build directories
llvm_src_dir = os.path.join(dir, "external", "llvm")
llvm_build_dir = os.path.join(llvm_src_dir, "build")
os.makedirs(llvm_build_dir, exist_ok=True)

# Define the URL and path for the LLVM tarball
llvm_tarball_url = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_VERSION}/llvm-{LLVM_VERSION}.src.tar.xz"
llvm_tarball = os.path.join(dir, "external", "llvm-{LLVM_VERSION}.src.tar.xz")

# Define CMake commands
cmake_cmd = [
    "cmake",
    llvm_src_dir,
    f"-DLLVM_TARGETS_TO_BUILD={LLVM_TARGETS}",
    f"-DLLVM_PARALLEL_COMPILE_JOBS={multiprocessing.cpu_count()}",
    f"-DLLVM_PARALLEL_LINK_JOBS={1 if multiprocessing.cpu_count() // 2 > 0 else multiprocessing.cpu_count() // 2}",
    f"-DCMAKE_BUILD_TYPE=Release",
    f"-DLLVM_ENABLE_ASSERTIONS=OFF",
    f"-DLLVM_INCLUDE_TESTS=OFF",
    f"-DLLVM_INCLUDE_EXAMPLES=OFF",
    f"-DLLVM_INCLUDE_BENCHMARKS=OFF",
    f"-DLLVM_BUILD_TOOLS=ON",
    f"-G Ninja"
]

# Define build and install commands
build_cmd = [
    "cmake", "--build", "."
]

inst_cmd = [
    "cmake",
    f"-DCMAKE_INSTALL_PREFIX={os.path.join(dir, 'build', 'llvm')}",
    "-P",
    "cmake_install.cmake"
]

import sys

try:
    with open(os.devnull, 'w') as devnull:
        if len(sys.argv) > 1 and sys.argv[1] == "--silent":
            if not os.path.exists(llvm_src_dir):
                print("Downloading LLVM source...")
                subprocess.run(["wget", llvm_tarball_url, "-O", llvm_tarball], check=True, stdout=devnull, stderr=devnull)
                subprocess.run(["tar", "-xf", llvm_tarball, "-C", dir], check=True, stdout=devnull, stderr=devnull)
                # Move the extracted source to the correct directory
                extracted_dir = os.path.join(dir, f"llvm-{LLVM_VERSION}.src")
                if os.path.exists(extracted_dir):
                    os.rename(extracted_dir, llvm_src_dir)

            subprocess.run(cmake_cmd, cwd=llvm_build_dir, check=True, stdout=devnull, stderr=devnull)
            x = subprocess.run(build_cmd, cwd=llvm_build_dir, check=True, stdout=devnull, stderr=devnull)
            if x.returncode == 0:
                subprocess.run(inst_cmd, cwd=llvm_build_dir, check=True, stdout=devnull, stderr=devnull)
                llvm_config_path = os.path.join(dir, "build", "llvm", "bin", "llvm-config")
                print(llvm_config_path)
        else:
            if not os.path.exists(llvm_src_dir):
                print("Downloading LLVM source...")
                subprocess.run(["wget", llvm_tarball_url, "-O", llvm_tarball], check=True)
                subprocess.run(["tar", "-xf", llvm_tarball, "-C", dir], check=True)
                # Move the extracted source to the correct directory
                extracted_dir = os.path.join(dir, f"llvm-{LLVM_VERSION}.src")
                if os.path.exists(extracted_dir):
                    os.rename(extracted_dir, llvm_src_dir)

            subprocess.run(cmake_cmd, cwd=llvm_build_dir, stderr=devnull, check=True)
            x = subprocess.run(build_cmd, cwd=llvm_build_dir, stderr=devnull, check=True)
            if x.returncode == 0:
                subprocess.run(inst_cmd, cwd=llvm_build_dir, check=True)
                llvm_config_path = os.path.join(dir, "build", "llvm", "bin", "llvm-config")
                print(llvm_config_path)
except subprocess.CalledProcessError as e:
    print(f"Error running cmake: {e}")