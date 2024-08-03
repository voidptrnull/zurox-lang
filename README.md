# Zurox Programming Language Suite

> Note: Zurox is still a WIP, things are worse than you might expect. Don't get your hopes too high, unless you feel like changing that.

## Dependencies

**Build system**: `CMake`

**Build Script Generator**: Any (`Ninja` is preffered)

These are the ones the scripts here will automatically handle:

- Google Test
- LLVM
- Z lib

These are **optional** but please try to use them for the sake of **convenience**:

- Clang (and rest of the stuff related to C/C++)
- LLVM LLD (For linking; Please don't use ld since the output is really a **HORRIBLE** experience with C++ names)

## Build Profiles

- **`Debug`**: Builds with minimal optimization and debug information.
- **`Release`**: Builds with expensive optimization and without debug information.
- **`RelWithDbgInfo`**: Builds with expensive optimization but keeps debug information.

>Note: Release profiles use IPO/LTO whenever supported.

## Build Arch (`-march` flags)

- x86_64
- x86_64-v3
- aarch64
- ppc64
- native (Default)

## Other options

- `ENABLE_TESTS` - Enable testing.
    - `ENABLE_VALGRIND` - Enable tests with valgrind.
- `STRIP_BINARIES` - Strip binaries when installing.
- `NO_OUTPUT_COLOR` - Disable compiler colored output for Ninja. Used for logging or piping output.
- `SOURCE_ZLIB` - Build ZLib from source.
- `SOURCE_LLVM` - Build LLVM from source.

## Building

In the project's root directory, run the following:
```sh
cmake -B build -G Ninja -DENABLE_TESTS=ON -DSTRIP_BINARIES=ON -DBUILD_ARCH=x86_64 -DCMAKE_BUILD_TYPE=Release
ninja -C build -j$(nproc) -l$(nproc)
```

## Installing

```sh
cmake --install build --prefix /usr
```

For most people that will either be:
```sh
sudo cmake --install build --prefix /usr
# OR
doas cmake --install build --prefix /usr
```