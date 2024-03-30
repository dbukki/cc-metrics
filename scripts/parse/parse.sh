#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift
db="$1"; shift

# Constants
null="/dev/null"
cc_parser="${HOME}/CodeCompassInstall/bin/CodeCompass_parser"
root_dir="${HOME}/CodeCompassProjects"
repo_dir="${root_dir}/Repos/${name}"
work_dir="${root_dir}/Workspace"
bld_dir="build"
cmp_cmds="compile_commands.json"
num_threads=4

# Enter repo dir
pushd "${repo_dir}" > $null

# Clear build dir
rm -rf "${bld_dir}"
mkdir "${bld_dir}"

# Reconstruct build dir
pushd "${bld_dir}" > $null
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
popd > $null

# Locate compile commands
if ! [ -f "${cmp_cmds}" ]; then
	cmp_cmds="${bld_dir}/${cmp_cmds}"
	if ! [ -f "${cmp_cmds}" ]; then
		echo "Error: Compile commands file not found."
		exit 1
	fi
fi

# Return to original dir
popd > $null

# Parse repo
"${cc_parser}" \
	-w "${work_dir}" \
	-n "${name}" \
	-i "${repo_dir}" \
	-i "${repo_dir}/${cmp_cmds}" \
	-d "${db}" \
	-j $num_threads \
	-f \
	"$@"
