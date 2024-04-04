#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
null="/dev/null"
root_dir="${HOME}/CodeCompassProjects"
repo_dir="${root_dir}/Repos/${name}"
parser="${cur_dir}/../parse/parse-sqlite.sh"
tag_log="cc-extract-tags.log"
ex_dir="${cur_dir}/../../artifacts/${name}"

# Ensure valid git repo
cd "${repo_dir}"
git status > $null
if [ $? -ne 0 ]; then
	echo "Error: Not a Git repository."
	exit 1
fi

# Ensure existence of tag list
if ! [ -f "${tag_log}" ]; then
	git tag --list > "${tag_log}"
	echo "Git tag list has been exported to ${tag_log}."
	echo "Make adjustments to the list now as you wish, then press ENTER to begin metrics extraction."
	read
fi

# Parse all tags one-by-one
while read tag; do
	echo -e "\n --- Parsing tag \"${tag}\" ---\n"
	git checkout "tags/${tag}"
	"${parser}" "${name}" --extract-to "${ex_dir}/${tag}"
done < "${tag_log}"
