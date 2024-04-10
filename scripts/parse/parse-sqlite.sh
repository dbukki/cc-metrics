#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
parser="${cur_dir}/parse.sh"
dbfile="ccdb.sqlite"
dbcs="sqlite:database=${dbfile}"

# Parse
rm "${dbfile}"
"${parser}" "${name}" "${dbcs}" "$@"
