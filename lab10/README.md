# Lab 10: Add Your Own System Call: trace

[Original PDF](Lab10_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

Adds per-process, inherited system-call tracing to xv6. `trace mask command [args...]` enables selected bits and executes the requested program.

## Files

- [patches/trace.patch](patches/trace.patch): complete user/kernel integration, including the Makefile entry.
- [patches/README.md](patches/README.md): patch base, changed files and reconstruction details.
- [user/trace.c](user/trace.c): the standalone user-program listing; the patch already creates the same file.
- [Makefile.inc](Makefile.inc): reference for manual integration; unnecessary when applying the complete patch.
- [xv6-commands.txt](xv6-commands.txt): runtime tests for masks, inheritance, error handling and isolation.
- [tests/test_trace_host.py](tests/test_trace_host.py): host checks using the actual patched dispatcher and handler with a mock kernel environment.
- [notes.md](notes.md): syscall numbers, masks, observations and limits.

## Apply and build in Ubuntu

The patch targets official [mit-pdos/xv6-riscv at f780d785](https://github.com/mit-pdos/xv6-riscv/tree/f780d7850765d74cbdcd1fff4b822d8829f7c9bd), which has `SYS_sync = 22`, `pause` and `printk`, matching the interfaces described in the PDF. Use a fresh checkout to avoid conflicts with earlier labs:

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git xv6-lab10
cd xv6-lab10
git checkout --detach f780d7850765d74cbdcd1fff4b822d8829f7c9bd
# Replace this absolute example path with your Operating-Systems checkout.
git apply --check /path/to/Operating-Systems/lab10/patches/trace.patch
git apply /path/to/Operating-Systems/lab10/patches/trace.patch
make qemu
```

The existing RISC-V toolchain, QEMU and xv6 build prerequisites are required. For a checkout containing earlier labs, check the patch before applying it and resolve any source-version differences explicitly. Do not replace the entire existing Makefile with `Makefile.inc`.

At the xv6 prompt:

```text
trace 32 grep hello README
trace 2147483647 grep hello README
trace 2 usertests forkforkfork
trace 65536 echo hi
trace 2129920 grep hello README
```

The first command selects `read`; the stress test checks inherited fork tracing; the last two select `write` and `open`/`close`. PIDs and read chunk sizes vary. Check the total read bytes against the actual README size and use [the remaining commands](xv6-commands.txt) for usage, failure and isolation checks.

## Host check

```bash
python3 /path/to/Operating-Systems/lab10/tests/test_trace_host.py /path/to/xv6-lab10 --cc gcc
```

This compiles and executes the actual dispatcher and `sys_trace` body with mock process state and syscall handlers. It checks bit selection, call names, printed return values, enabling/disabling tracing and invalid syscall numbers. It does not execute the RISC-V kernel or test real forks, traps, scheduling or process-slot reuse. Those require QEMU.

## Reconstruction

The PDF remains unchanged; DOCX and Markdown are extracted versions. The patch reconstructs the excerpts against a fixed official source revision. Added initialization/reset prevents trace masks leaking through reused process slots. Mask selection uses an unsigned shift with a width guard. These additions are documented separately from the original recorded results.

See [repository validation](../IMPLEMENTATION.md) for checks actually performed.
