#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
filter="$1"; shift

# Constants
art_dir="${cur_dir}/../../artifacts"
sum_exe="${cur_dir}/../../build/ccexsum"
table_file="cc-results-table.txt"
reg_file="cc-tables.txt"
res_file="cc-summary.txt"

# Create summary registry (discover tables)
cd "${art_dir}"
find . -wholename "*/${filter}/${table_file}" > "${reg_file}"

# Summarize results of comparisons
"${sum_exe}" "${reg_file}" " & " " \\\\\\\\\n" | tee "${res_file}"
