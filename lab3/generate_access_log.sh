#!/usr/bin/env bash
# Reconstructed from the data-preparation loop in the Lab 3 PDF.
set -eu
if [ "$#" -gt 1 ]; then
    echo "usage: generate_access_log.sh [output.log]" >&2
    exit 1
fi
output=access.log
if [ "$#" -eq 1 ]; then output=$1; fi
if [ -e "$output" ]; then
    echo "output already exists: $output" >&2
    exit 1
fi
for i in $(seq 1 500); do
    status=OK
    if [ $((i % 5)) -eq 0 ]; then status=FAIL; fi
    printf '2026-10-05 10:%02d user%d %s /page%d\n' \
        "$((i % 60))" "$((i % 7))" "$status" "$((i % 4))"
done > "$output"
