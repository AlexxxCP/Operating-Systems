#!/usr/bin/env bash
# Reproduce the three strace experiments in a fresh directory.
set -euo pipefail
if [[ $(uname -s) != Linux ]]; then
    echo 'This experiment requires Linux strace.' >&2; exit 1
fi
command -v strace >/dev/null
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
workdir=$(mktemp -d)
echo "Experiment outputs: $workdir"
cd "$workdir"
strace -f -e trace=openat,read,write,close -o part1-trace.txt ls -l > part1-output.txt
"${CC:-cc}" -std=c99 -Wall -Wextra -Werror "$root/syscalls.c" -o syscalls
strace -e trace=openat,write,read,close -o part2-trace.txt ./syscalls > part2-output.txt
strace -f -c -e trace=openat -o part3-count.txt ls -l > part3-output.txt
for name in openat read write close; do
    printf '%s: ' "$name"
    grep -cE "(^|[[:space:]])$name\(" part1-trace.txt || true
done
cat part2-output.txt
cat part3-count.txt
