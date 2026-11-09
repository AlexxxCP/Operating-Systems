# Executed validation — 9 November 2026

Both jobs passed in [GitHub Actions run 42682578993](https://github.com/AlexxxCP/Operating-Systems/actions/runs/42682578993).

Tested code: [74b5f108b2f00161657d79a2c9e569cfd442318e](https://github.com/AlexxxCP/Operating-Systems/commit/74b5f108b2f00161657d79a2c9e569cfd442318e). xv6 base: [f780d7850765d74cbdcd1fff4b822d8829f7c9bd](https://github.com/mit-pdos/xv6-riscv/tree/f780d7850765d74cbdcd1fff4b822d8829f7c9bd). Later documentation-only changes do not alter the tested programs.

| Work | Executed checks | Result |
| --- | --- | --- |
| Lab 2 | Actual sleep executable in xv6; minimum elapsed clock ticks, zero/leading zeros, missing/extra arguments, invalid text, negatives, overflow, empty input and whitespace. Host test also checks INT_MAX and syscall failure propagation. | Passed |
| Lab 7 | Ubuntu builds of all five C programs; successful/failed exec, redirection, both pipelines compared with native shell commands, actual zombie state and reaping. | Passed |
| Lab 8 | Actual xv6 echo/ls exec, failed exec with status 1, three ping/pong exchanges with distinct PIDs and required order. | Passed |
| Lab 9 | Real Linux strace; file truncation, exact 14-byte round trip, openat/write/read entries. | Passed |
| Lab 10 | Real patched kernel; zero/read/all/write/open-close masks, exact README read total and EOF, invalid masks, usage and exec errors, inherited/disabled tracing, fork stress and shell isolation. Host dispatcher checks also passed. | Passed |

The workflow built the RISC-V kernel and user programs with GCC/binutils and booted them in QEMU on Ubuntu. Its artifact `xv6-test-logs` contains `build.txt`, `qemu.txt` and `result.txt`.

[qemu.txt](qemu.txt) preserves that run's console content with line endings normalized to LF, so it remains available after artifact expiration. It is new execution evidence, separate from the older outputs preserved in the laboratory PDFs.

Selected actual output:

```text
sleeptest: ALL TESTS PASSED

$ run nosuchprog
parent: my pid is 18
child: my pid is 19
exec failed
parent: child 19 ended with status 1

$ pingpong
21: received ping
20: received pong

$ trace 32 grep hello README
27: syscall read -> 1023
27: syscall read -> 965
27: syscall read -> 453
27: syscall read -> 0

$ tracecheck
tracecheck: parent 36
parent marker
36: syscall write -> 14
child marker
37: syscall write -> 13
child-disabled
tracecheck: child 37
tracecheck: ALL TESTS PASSED
```

The three positive read results total 2441 bytes, the README size in the pinned source. PIDs, host directory counts and scheduling can vary on a later run.

To repeat the checks, use [the workflow](../.github/workflows/labs.yml), [the Linux Lab 7 test](../lab7/tests/test.sh), [the Linux Lab 9 test](../lab9/tests/test.sh) and [the QEMU runner](../scripts/test_xv6.py). The QEMU runner requires a clean checkout of the pinned xv6 revision and builds in a temporary copy.
