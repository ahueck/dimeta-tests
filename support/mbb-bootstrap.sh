#!/bin/bash

export CC=clang
export CXX=clang++

script_dir=$(pwd)

if [ -z "$1" ]; then
    base_mbb_f=$(pwd)
else
    base_mbb_f="$1"
fi

build_f="$base_mbb_f"/mbb

echo "Base folder $base_mbb_f"
echo "Build to $build_f"

function mbb_requirements() {
  cd "$build_f"  
  python3 -m venv .venv
  source .venv/bin/activate
  pip install tqdm pandas typing_extensions
}

function mbb_fetch() {
    cd "$base_mbb_f"
    git clone git@git-ce.rwth-aachen.de:hpc-public/mpi-bugbench.git mbb
}

function mbb_patch() {
    cd "$build_f"
    git apply "${script_dir}"/mbb.patch
}

function mbb_generate() {
    cd "$build_f"
    source .venv/bin/activate
    python3 scripts/generate.py --filter dtype --level 3
}

mbb_fetch
mbb_patch
mbb_requirements
mbb_generate

echo "##########"
#echo "Execute: export PATH="$install_f"/bin:\$PATH"
#echo "Execute: export MBB_PATH="$install_f""

