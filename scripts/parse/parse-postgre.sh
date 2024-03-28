#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
parser="${cur_dir}/parse.sh"
db="pgsql:host=localhost;port=5432;user=compass;password=ccdb;database=ccdb"

# Parse
"${parser}" "${name}" "${db}" "$@"

