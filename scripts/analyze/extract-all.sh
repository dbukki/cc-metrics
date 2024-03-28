#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
root_dir="${HOME}/CodeCompassProjects"
repo_dir="${root_dir}/Repos/${name}"
parser="${cur_dir}/../parse/parse-sqlite.sh"
tag_log="cc-extract-tags.log"
ex_dir="${cur_dir}/../../artifacts/${name}"

# Ensure existence tag list
cd "${repo_dir}"
if ! [ -f "${tag_log}" ]; then
	git tag --list > "${tag_log}"
fi

# Parse all tags one-by-one
cat "${tag_log}" | while read tag; do
	echo "\n --- Parsing tag \"${tag}\" ---\n"
	git checkout "tags/${tag}"
	"${parser}" "${name}" --extract-to "${ex_dir}/${tag}"
done
