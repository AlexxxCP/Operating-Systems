#!/usr/bin/env bash
# Transcribed from Lab 3, with an explicit check for a missing argument.
if [ "$#" -ne 1 ]; then
    echo "usage: report.sh access.log" >&2
    exit 1
fi
log=$1
if [ ! -f "$log" ]; then
    echo "no such file: $log" >&2
    exit 1
fi
total=$(wc -l < "$log")
fails=$(grep -c FAIL "$log")
echo "$total requests, $fails failed"
for i in $(seq 0 6); do
    n=$(grep -c "user$i FAIL" "$log")
    echo "user$i: $n failures"
done
