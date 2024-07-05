
#!/bin/sh
rm build -rf

echo "Enter build type(R for Release, anything else for Debug): "
read BT

if [[ "$BT" == "R" ]]; then
    BT="Release"
else
    BT="Debug"
fi

echo "Enter build architecture(A for all, ARCH for the architecture to build for): "
read BA

arch_list=("x86-64" "x86-64-v3" "aarch64" "powerpc64" "native")

case "$BA" in
    x86-64|x86-64-v3|aarch64|powerpc64|native)
        echo "Match found: $BA"
        rm build -rf
        cmake -B build -DCMAKE_BUILD_TYPE=$BT -DBUILD_ARCH=$BA -G Ninja
        ninja -C build -j$(nproc)
        cmake --install build/ --prefix ./bin/$BA/
    ;;
    *)
        echo "No match found for '$BA' or 'A' is provided, assuming it to be 'A' anyways."
        echo "NOTE: BUILDS MIGHT FAIL, AND BINARIES WILL BE MADE IN $(pwd)/bin"
        for i in "${arch_list[@]}"; do
            rm build -rf
            cmake -B build -DCMAKE_BUILD_TYPE=$BT -DBUILD_ARCH=$i -G Ninja
            ninja -C build -j$(nproc)
            cmake --install build/ --prefix ./bin/$i/
        done
    ;;
esac