#!/usr/bin/env bash
# Reconstructed from Lab 5. Files are created in a fresh temporary directory.
set -eu
if [ "$(uname -s)" != Linux ] || [ "$(id -u)" -eq 0 ]; then
    echo "Run in Linux as an ordinary user to observe permission errors." >&2
    exit 1
fi
workdir=$(mktemp -d)
echo "Experiment directory: $workdir"
cd "$workdir"
ls -ld /etc /tmp /bin
ls -l /etc/passwd /dev/null
readlink /bin
readlink -f /bin
du -sh /usr /var/log 2>/dev/null || true
echo hello > notes.txt
stat notes.txt
ls -li notes.txt
ln notes.txt hard.txt
ls -li notes.txt hard.txt
rm notes.txt
cat hard.txt
ln -s hard.txt soft.txt
ls -li hard.txt soft.txt
cat soft.txt
rm hard.txt
cat soft.txt || true
ls -l soft.txt
mkdir d
ls -ld d
mkdir d/x d/y
ls -ld d
echo x > one.txt
ls -l one.txt
du -h one.txt
truncate -s 1G big.img
ls -lh big.img
du -h big.img
echo secret > s.txt
chmod 600 s.txt
ls -l s.txt
chmod 000 s.txt
cat s.txt || true
chmod u+r s.txt
cat s.txt
umask
touch new.txt
mkdir newdir
ls -ld new.txt newdir
mkdir box
echo hi > box/f
trap 'chmod 700 box 2>/dev/null || true' EXIT
chmod u=r,go= box
ls box
cat box/f || true
chmod u=x,go= box
ls box || true
cat box/f
chmod 700 box
trap - EXIT
ls -l /usr/bin/passwd
ls -ld /tmp
# Inspect protected-file metadata without printing password hashes.
ls -l /etc/shadow
lsblk
df -h
mount | grep ' / '
grep -c processor /proc/cpuinfo
head -3 /proc/meminfo
head -5 /proc/self/status
ls -l /proc/self/fd
