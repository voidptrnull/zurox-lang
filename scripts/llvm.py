#!/usr/bin/env python

import os
import subprocess
import multiprocessing

LLVM_TARGETS = "X86;ARM;PowerPC"
LLVM_PROJ = "lld"

dir = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir))

llvm_build_dir = os.path.join(dir, "external", "llvm", "build")
os.makedirs(llvm_build_dir, exist_ok=True)

cmake_cmd = [
	"cmake",
	f"{os.path.join(dir, "external", "llvm", "llvm")}",
	f"-DLLVM_TARGETS_TO_BUILD={LLVM_TARGETS}",
	f"-DLLVM_ENABLE_PROJECTS={LLVM_PROJ}",
	f"-DLLVM_BUILD_LLVM_DYLIB=ON",
	f"-DLLVM_LINK_LLVM_DYLIB=ON",
	f"-DLLVM_PARALLEL_COMPILE_JOBS={multiprocessing.cpu_count()}",
	f"-DLLVM_PARALLEL_LINK_JOBS={1 if multiprocessing.cpu_count() // 2 > 0 else multiprocessing.cpu_count() // 2}",
	f"-DCMAKE_BUILD_TYPE=Release",
	f"-DLLVM_ENABLE_ASSERTIONS=OFF",  
	f"-DLLVM_INCLUDE_TESTS=OFF",  
	f"-DLLVM_INCLUDE_EXAMPLES=OFF",  
	f"-DLLVM_INCLUDE_BENCHMARKS=OFF", 
	f"-DLLVM_ENABLE_LLD=ON",
	f"-DLLVM_BUILD_TOOLS=ON",
	f"-G Ninja"
]

build_cmd = [
	"cmake", "--build", "."
]

if not os.path.exists(os.path.join(dir, "build")):
	os.mkdir(os.path.join(dir, "build"))

inst_cmd = [
	"cmake",
	f"-DCMAKE_INSTALL_PREFIX={os.path.join(dir, "build", "llvm")}",
	"-P",
	"cmake_install.cmake"
]

import sys

try:
	with open(os.devnull, 'w') as devnull:
		if len(sys.argv) > 1 and sys.argv[1] == "--silent":
			subprocess.run(cmake_cmd, cwd=llvm_build_dir, check=True, stdout=devnull, stderr=devnull)
			x = subprocess.run(build_cmd, cwd=llvm_build_dir, check=True, stdout=devnull, stderr=devnull)
			if x.returncode == 0:
				subprocess.run(inst_cmd, cwd=llvm_build_dir, check=True, stdout=devnull, stderr=devnull)
				llvm_config_path = os.path.join(dir, "build", "llvm", "bin", "llvm-config")
				print(llvm_config_path)
		else:
			subprocess.run(cmake_cmd, cwd=llvm_build_dir, stderr=devnull, check=True)
			x = subprocess.run(build_cmd, cwd=llvm_build_dir, stderr=devnull, check=True)
			if x.returncode == 0:
				subprocess.run(inst_cmd, cwd=llvm_build_dir, check=True)
				llvm_config_path = os.path.join(dir, "build", "llvm", "bin", "llvm-config")
				print(llvm_config_path)
except subprocess.CalledProcessError as e:
	print(f"Error running cmake: {e}")
