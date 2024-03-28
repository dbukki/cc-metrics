#!/bin/bash

# Parameters
cur_path=$(readlink -f "$0")
cur_dir=$(dirname "${cur_path}")
name="$1"; shift

# Constants
parser="${cur_dir}/parse.sh"
websrv="${HOME}/CodeCompassInstall/bin/CodeCompass_webserver"
work_dir="${HOME}/CodeCompassProjects/Workspace"
port=11111
url="http://localhost:${port}/new/project/?workspaceId=${name}"

# Run web server
xdg-open "${url}"
"${websrv}" \
	-w "${work_dir}" \
	-p $port

