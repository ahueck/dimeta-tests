#!/bin/bash

function dimeta_modules() {
    module purge
    ml gcc/11 git python
    ml clang/14
}

export CC=clang
export CXX=clang++

if [ -z "$1" ]; then
    base_dimeta_f=$(pwd)
else
    base_dimeta_f="$1"
fi

build_f="$base_dimeta_f"/dimeta/build
install_f="$base_dimeta_f"/dimeta/install

echo "Base folder $base_dimeta_f"
echo "Build to $build_f ; Install to $install_f"

function dimeta_fetch() {
    cd "$base_dimeta_f"
    git clone --depth 1 --branch v0.3.0  https://github.com/ahueck/llvm-dimeta.git dimeta
}

function dimeta_config() {
    mkdir -p "$build_f"
    cd "$build_f"
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$install_f" \
        -DCMAKE_BUILD_TYPE=Release 
}

function dimeta_install() {
    cd "$build_f"
    make -j16 install
}

dimeta_fetch
dimeta_config
dimeta_install

echo "##########"
echo "Execute: export PATH="$install_f"/bin:\$PATH"
echo "Execute: export DIMETA_PATH="$install_f""
