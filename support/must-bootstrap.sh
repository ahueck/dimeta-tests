#!/bin/bash

function must_modules() {
    module purge
    ml gcc/11 cuda openmpi git python
    ml clang/14
}

export CC=clang
export CXX=clang++

script_dir=$(pwd)

if [ -z "$1" ]; then
    base_must_f=$(pwd)
else
    base_must_f="$1"
fi

build_f="$base_must_f"/must-dimeta/build
install_f="$base_must_f"/must-dimeta/install

echo "Base folder $base_must_f"
echo "Build to $build_f ; Install to $install_f"

function must_fetch() {
    cd "$base_must_f"
    git clone -b feature/typeart-fidelity --single-branch git@git-ce.rwth-aachen.de:hpc-research/correctness/MUST.git must-dimeta
    cd "$base_must_f"/must-dimeta
    git submodule update --recursive --init
}

function must_download() {
    cd "$base_must_f"
}

function must_config() {
    mkdir -p "$build_f"
    cd "$build_f"
    cmake ../ \
        -DCMAKE_INSTALL_PREFIX="$install_f" \
        -DENABLE_TYPEART=ON \
        -DUSE_BACKWARD=False \
        -DTYPEART_SOFTCOUNTERS=ON \
        -DCMAKE_BUILD_TYPE=Release
}

function must_install() {
    cd "$build_f"
    make -j32 install install-prebuilds
}

must_fetch
#must_patch
#must_download
must_config
must_install

echo "##########"
echo "Execute: export PATH="$install_f"/bin:\$PATH"
echo "Execute: export MUST_PATH="$install_f""
