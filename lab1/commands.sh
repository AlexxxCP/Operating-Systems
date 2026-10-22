#!/usr/bin/env bash
# Reconstructed from Lab 1; uses a fresh temporary directory and its own PID.
set -eu
if [ "$(uname -s)" != Linux ]; then
    echo "Run this experiment in Linux (for example, the Ubuntu VM)." >&2
    exit 1
fi
workdir=$(mktemp -d)
echo "Experiment directory: $workdir"
cd "$workdir"
whoami
uname -a
uptime
pwd
ls -la /
ls -la ~
(cd /etc && ls | head)
mkdir demo
cd demo
echo "hello operating systems" > note.txt
cp note.txt copy.txt
mv copy.txt renamed.txt
ls -l
rm renamed.txt
chmod 600 note.txt
ls -l note.txt
ps aux | head
ps aux | wc -l
sleep 300 &
sleep_pid=$!
trap 'kill "$sleep_pid" 2>/dev/null || true' EXIT
jobs
ps -ef | grep '[s]leep'
ls "/proc/$sleep_pid/"
head -20 "/proc/$sleep_pid/status"
free -h
head -6 /proc/meminfo
grep VmRSS "/proc/$sleep_pid/status"
kill "$sleep_pid"
wait "$sleep_pid" 2>/dev/null || true
trap - EXIT
df -h
lsblk
du -sh "$workdir"
ls -l /dev | head
mount | head
