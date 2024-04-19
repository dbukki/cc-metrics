#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
art_dir="${cur_dir}/../../artifacts/${name}"
cmp_exe="${cur_dir}/../../build/ccexcmp"
acc_exe="${cur_dir}/../../build/ccexacc"
cmp_list="cc-compare.txt"
cmp_ext=".log"
col_file="cc-columns.txt"
res_file="cc-results.txt"

# Ensure existence of compare list
cd "${art_dir}"
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
cols=""
prev=""
while read next; do
    if ! [ -z "${next}" ]; then
        if ! [ -z "${prev}" ]; then
            echo -e "\n --- Comparing \"${prev}\" with \"${next}\"  ---\n"
            file="${prev}-${next}${cmp_ext}"
            "${cmp_exe}" "${prev}" "${next}" | tee "${file}"
            cols="${cols}${file}\n"
        fi
        prev="${next}"
    fi
done < "${cmp_list}"
echo -e "${cols}" > "${col_file}"

# Accumulate results of comparisons
echo -e "\n --- Accumulating results into \"${res_file}\" ---\n"
"${acc_exe}" "${col_file}" " & " " \\\\\\\\\n" | tee "${res_file}"
