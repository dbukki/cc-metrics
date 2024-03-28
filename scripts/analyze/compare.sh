#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
left="$1"; shift
right="$1"; shift

# Constants
cmp_exe="${cur_dir}/../../build/ccexcmp"

# Compare
"${cmp_exe}" "${left}" "${right}" | tee "${left}-${right}.log"
