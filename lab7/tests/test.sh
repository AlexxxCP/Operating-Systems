#!/usr/bin/env bash
# Linux integration checks. All artifacts are confined to a new temporary directory.
set -euo pipefail
if [[ $(uname -s) != Linux ]]; then
    echo 'These process and /proc tests require Linux.' >&2
    exit 1
fi
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
workdir=$(mktemp -d)
zombie_pid=''
cleanup() {
    if [[ -n $zombie_pid ]]; then wait "$zombie_pid" 2>/dev/null || true; fi
    rm -rf -- "$workdir"
}
trap cleanup EXIT
for program in run zombie redirect pipeline pipeline3; do
    "${CC:-cc}" -std=c99 -Wall -Wextra -Werror "$root/$program.c" -o "$workdir/$program"
done
cd "$workdir"
timeout 5 ./run > run.txt
grep -q 'ended with status 0' run.txt
timeout 5 ./run date > date.txt
if timeout 5 ./run __os_labs_missing_command__ > missing.txt 2> missing.err; then
    echo 'Missing executable unexpectedly succeeded.' >&2; exit 1
else
    status=$?
    [[ $status -eq 127 ]]
fi
grep -q 'ended with status 127' missing.txt
timeout 5 ./redirect > redirect.txt 2> redirect.err
grep -q 'open gave descriptor 1' redirect.err
grep -q 'run.c\|run.txt\|redirect' out.txt
[[ $(timeout 5 ./pipeline | tr -d '[:space:]') == $(ls /etc | wc -l | tr -d '[:space:]') ]]
[[ $(timeout 5 ./pipeline3 | tr -d '[:space:]') == $(ls /usr/bin | grep x | wc -l | tr -d '[:space:]') ]]
./zombie > zombie.txt &
zombie_pid=$!
observed=0
for ((attempt=0; attempt<40; attempt++)); do
    if ps -o stat= --ppid "$zombie_pid" | grep -q '^Z'; then observed=1; break; fi
    sleep 0.1
done
[[ $observed -eq 1 ]]
child_pid=$(ps -o pid= --ppid "$zombie_pid" | tr -d '[:space:]')
wait "$zombie_pid"
zombie_pid=''
[[ ! -e /proc/$child_pid ]]
echo 'PASS: run, failed exec, redirection, both pipelines, zombie creation and reaping.'
