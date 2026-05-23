#!/bin/bash

if [ ! -z "$1" ]; then
  typeart_branch="${1}"
  echo "Using typeart branch ${typeart_branch}"
  skip_patch=0
else
  skip_patch=1
fi

script_dir=$(pwd)
base_must_f=$(pwd)


build_f="$base_must_f"/must-dimeta/build
install_f="$base_must_f"/must-dimeta/install

echo "Base folder $base_must_f"
echo "Build to $build_f ; Install to $install_f"

function must_modules() {
    module purge
    ml gcc/11 cuda openmpi git python
    ml clang/18
}

function must_fetch() {
    cd "$base_must_f"
    git clone --branch develop  git@git-ce.rwth-aachen.de:hpc-research/correctness/MUST.git must-dimeta
    cd must-dimeta
    git submodule update --init --recursive
}

function must_download() {
    cd "$base_must_f" || return
    if [ -d "must-dimeta" ]; then
        echo "Folder 'must-dimeta' already exists. Skipping download."
        return
    fi
    local must_version=MUST-v1.11.2
    if [ ! -f "$base_must_f"/"${must_version}.tar.gz" ]; then
        wget https://hpc.rwth-aachen.de/must/files/${must_version}.tar.gz
    fi
    tar -xzf ${must_version}.tar.gz
    mv ${must_version} must-dimeta
}

function must_patch_typeart() {
    cd "$base_must_f/must-dimeta/externals" || return
    if [ -d "typeart" ]; then
        echo "Removing existing 'typeart' folder."
        rm -rf typeart/
    fi
    git clone -b ${typeart_branch} https://github.com/tudasc/TypeART.git typeart
}

function must_config() {
    if [ -d "$build_f" ]; then
        echo "Build folder already exists."
        rm -rf "$build_f"
    fi
    if [ -d "$install_f" ]; then
        echo "Install folder already exists."
        rm -rf "$install_f"
    fi

    mkdir -p "$build_f"
    cd "$build_f"
    cmake ../ \
        -DCMAKE_INSTALL_PREFIX="$install_f" \
        -DENABLE_TYPEART=ON \
        -DTYPEART_SOFTCOUNTERS=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DENABLE_FORTRAN=OFF \
        -DUSE_BACKWARD=OFF 
}

function must_install() {
    cd "$build_f"
    make -j10 install install-prebuilds
}

#must_download
must_fetch
if [[ "$skip_patch" != 1 ]]; then
  must_patch_typeart
fi
must_config
must_install

echo "##########"
echo "Execute: export PATH="$install_f"/bin:\$PATH"
echo "Execute: export MUST_PATH="$install_f""
