#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
dir="$1"; shift

# Constants
cmp_exe="${cur_dir}/compare.sh"

# Compare
cd "${dir}"
prev=""
ls -d "*/" | while read dir; do
    if ! [ -z "${prev}" ]; then
        "${cmp_exe}" "${prev}" "${dir}"
    fi
    prev="${dir}"
done
