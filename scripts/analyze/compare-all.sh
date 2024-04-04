#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
dir="$1"; shift

# Constants
cmp_exe="${cur_dir}/compare.sh"
cmp_list="cc-compare.txt"

# Ensure existence of compare list
cd "${dir}"
if ! [ -f "${cmp_list}" ]; then
    list=""
    while read tag; do
        list="${list}${tag%/}\n"
    done < <(ls -d */)
    echo -e "${list}" > "${cmp_list}"
    echo "Tag compare list has been exported to ${cmp_list}."
    echo "Make adjustments to the list now as you wish, then press ENTER to begin comparison."
    read
fi

# Compare consecutive tags in the list
prev=""
while read next; do
    if ! [ -z "${next}" ]; then
        if ! [ -z "${prev}" ]; then
            echo -e "\n --- Comparing \"${prev}\" with \"${next}\"  ---\n"
            "${cmp_exe}" "${prev}" "${next}"
        fi
        prev="${next}"
    fi
done < "${cmp_list}"
