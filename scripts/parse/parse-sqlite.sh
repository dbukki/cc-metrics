#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
parser="${cur_dir}/parse.sh"
db="sqlite:database=ccdb.sqlite"

# Parse
"${parser}" "${name}" "${db}" "$@"

