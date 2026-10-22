#!/usr/bin/env bash
# Q1-Q8 reconstructed from the Lab 3 report. Q7-Q8 depend on the host OS.
if [ "$#" -ne 1 ] || [ ! -f "$1" ]; then
    echo "usage: oneliners.sh access.log" >&2
    exit 1
fi
log=$1
echo 'Q1: failed requests'
grep -c FAIL "$log"
echo 'Q2: distinct pages'
echo "count: $(cut -d' ' -f5 "$log" | sort -u | wc -l)"
cut -d' ' -f5 "$log" | sort -u
echo 'Q3: requests per page'
cut -d' ' -f5 "$log" | sort | uniq -c | sort -rn
echo 'Q4: users with the most failures'
grep FAIL "$log" | cut -d' ' -f3 | sort | uniq -c | sort -rn | awk 'NR==1{m=$1} $1==m'
echo 'Q5: user3 requests'
echo "total: $(grep -c ' user3 ' "$log"), failed: $(grep ' user3 ' "$log" | grep -c FAIL)"
echo 'Q6: last three failures, time and user'
grep FAIL "$log" | tail -3 | cut -d' ' -f2,3
echo 'Q7: login shells'
cut -d: -f7 /etc/passwd | sort | uniq -c | sort -rn
echo 'Q8: listing line counts'
ls /etc | wc -l
ls -l /etc | wc -l
