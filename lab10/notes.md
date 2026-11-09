# Lab 10 observations

## Masks in this xv6 revision

| Mask | Selection |
| --- | --- |
| 0 | No tracing. |
| 2 | Bit 1: `fork`. |
| 32 | Bit 5: `read`. |
| 65536 | Bit 16: `write`. |
| 2129920 | Bits 15 and 21: `open` and `close`. |
| 2147483647 | Bits 0–30, covering the currently registered calls. |

`SYS_trace` is 23 because `sync` already occupies 22. `trace` stores a mask in the current process; `exec` keeps that process and its mask; `kfork` copies the mask into its child. Fresh or recycled process slots start with mask zero.

Tracing occurs after the syscall handler returns, so its result is already in `a0`. A non-returning call such as successful `exit` cannot produce an after-return trace line. Kernel `printk` output does not invoke the user `write` syscall, avoiding recursive tracing.

## Recorded results from the PDF

- Before adding the kernel handler, the user wrapper reached unknown syscall 23. After registration the command worked.
- The read-only example returned 1023, 965, 453 and then 0 bytes. The positive results sum to the recorded README size of 2441; 0 means EOF. A different README or buffering pattern can change individual results.
- All-call tracing showed `trace`, `exec`, `open`, `read` and `close` in the recorded run.
- `trace 2 usertests forkforkfork` emitted traces from descendant PIDs and eventually reported `ALL TESTS PASSED`. Fork failures of -1 occurred at the process-table limit. This is a result recorded in the PDF, not a claim about a new QEMU run.
- `echo hi` used separate writes for the characters and newline. The kernel trace appeared between them.
- The combined mask `(1 << 15) | (1 << 21)` printed only `open` and `close`.

The reconstructed patch additionally initializes/resets masks and avoids signed/out-of-range shifts. The host test exercises dispatcher logic with mocks; full inheritance and process isolation still need the runtime checks in [xv6-commands.txt](xv6-commands.txt).

The reviewed wrapper accepts decimal masks from 0 to 2147483647 and rejects invalid or overflowing values. The [automated QEMU runner](../scripts/test_xv6.py) also executes the runtime checks and a dedicated [inheritance helper](tests/tracecheck.c). Their actual outcome is recorded by GitHub Actions and linked in the repository validation notes.
