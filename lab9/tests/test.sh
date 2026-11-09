#!/usr/bin/env bash
# Linux integration check: file bytes, truncation and observed kernel calls.
set -euo pipefail
if [[ $(uname -s) != Linux ]]; then
    echo 'These syscall-tracing tests require Linux.' >&2; exit 1
fi
command -v strace >/dev/null
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
workdir=$(mktemp -d)
trap 'rm -rf -- "$workdir"' EXIT
"${CC:-cc}" -std=c99 -Wall -Wextra -Werror "$root/syscalls.c" -o "$workdir/syscalls"
cd "$workdir"
printf 'old data that must be truncated and replaced\n' > note.txt
timeout 5 strace -e trace=openat,write,read,close -o trace.txt ./syscalls > output.txt
printf 'hello, kernel\n' > expected.txt
cmp expected.txt note.txt
[[ $(wc -c < note.txt) -eq 14 ]]
grep -q 'write sent 14 bytes' output.txt
grep -q 'read got 14 bytes: hello, kernel' output.txt
grep -qE 'openat\(.*"note.txt".*O_TRUNC' trace.txt
grep -qE 'write\([0-9]+, "hello, kernel\\n", 14\).*=[[:space:]]*14' trace.txt
grep -qE 'read\([0-9]+, "hello, kernel\\n", 63\).*=[[:space:]]*14' trace.txt
echo 'PASS: truncation, exact 14-byte round trip, openat/write/read tracing.'
