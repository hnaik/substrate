#!/bin/bash

set -euo pipefail

task=${1:-build}
build_type=${2:-debug}
toolchain=${3:-gcc}

prefix=
os_type=$(uname)

if which "pixi" >/dev/null; then
    if [ $os_type == "Darwin" ]; then  
        prefix="pixi run -e macosx"
    else
        prefix="pixi run"
    fi
fi

src_dir=$(realpath $(dirname $0)/..)
build_dir="${src_dir}/build/${toolchain}-${build_type}"

if [ $task == "config" ]; then
    set -x
    $prefix cmake -G Ninja \
        -S=${src_dir} -B=${build_dir} \
        -DCMAKE_BUILD_TYPE=${build_type}
    { set +x; } 2>/dev/null
elif [ $task == "build" ]; then
    set -x
    $prefix cmake --build ${build_dir}
    { set +x; } 2>/dev/null
fi
