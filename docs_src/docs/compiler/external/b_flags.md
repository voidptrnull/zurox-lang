# Flags
## Overview

The Zurox Compiler (zuroxc) allows you to compile Zurox source files into various stages such as object files, assembly code, and LLVM IR. The following flags can be used to control the behavior of the compiler.

## Command Line Options
General Options

- `-o` <br>
    Description: Specify the name of the output file.
    Usage:

    ```sh
    zuroxc -o outputfile main.zx
    ```

- `--version` <br>
    Description: Display the version of the Zurox Compiler.
    Usage:

    ```sh
    zuroxc --version
    ```
- `--help` <br>
    Description: Display the help message with all available options.
    Usage:

    ```sh
    zuroxc --help
    ```

## Stage Type Options

- `-c` <br>
    Description: Run all stages except linking. This flag compiles the source files into object files.
    Usage:

    ```sh
    zuroxc -c main.zx
    ```

- `-S` <br>
    Description: Compile source files to assembly code.
    Usage:

    ```sh
    zuroxc -S main.zx
    ```

- `-B` <br>
    Description: Output the LLVM IR for the source files.
    Usage:

    ```sh
    zuroxc -B main.zx
    ```
-C
Description: Check if the code compiles without producing any output files.
Usage:

sh

    zuroxc -C main.zx

## Optimization Level Options

    -g
    Description: No optimizations, enable debugging.
    Usage:

    sh

zuroxc -g main.zx

-O0
Description: Perform no optimizations.
Usage:

sh

zuroxc -O0 main.zx

-O1
Description: Enable trivial optimizations.
Usage:

sh

zuroxc -O1 main.zx

-O2
Description: Enable default optimizations.
Usage:

sh

zuroxc -O2 main.zx

-O3
Description: Enable expensive optimizations.
Usage:

sh

    zuroxc -O3 main.zx

Target Architecture Option

    -march <architecture name>
    Description: Choose the target architecture for code generation.
    Usage:

    sh

    zuroxc -march x86-64 main.zx

Example Usage

sh

# Compile main.zx to an object file without linking
zuroxc -c main.zx

# Compile main.zx to assembly code
zuroxc -S main.zx

# Output LLVM IR for main.zx
zuroxc -B main.zx

# Check if main.zx compiles without producing any output files
zuroxc -C main.zx

# Compile main.zx with default optimizations
zuroxc -O2 main.zx

# Compile main.zx for a specific architecture
zuroxc -march armv7 main.zx
