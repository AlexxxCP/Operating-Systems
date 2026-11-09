# Lab 8: The Same Calls, Inside Your Own OS

[Original PDF](Lab8_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

Process creation and a two-way one-byte exchange inside xv6.

## Files

- [user/run.c](user/run.c): `fork`, `exec` and `wait`; defaults to `echo hello from the child`.
- [user/pingpong.c](user/pingpong.c): two pipes, one child and one reply byte.
- [Makefile.inc](Makefile.inc): `UPROGS` additions for the existing xv6 build.
- [xv6-commands.txt](xv6-commands.txt): runtime checks to enter at the xv6 prompt.
- [notes.md](notes.md): all Part 1 and Part 2 observation answers.

## Build and run

Copy `user/run.c` and `user/pingpong.c` into the `user/` directory of the matching xv6-riscv checkout. Add `$U/_run` and `$U/_pingpong` to its existing `UPROGS` list, or copy `Makefile.inc` into that checkout and include it **after** the list definition and **before** the `fs.img` rule. Make expands the prerequisite list when reading that rule. Keep one entry per program.

Run `make qemu` in the xv6 checkout, then enter:

```text
run
run ls
run nosuchprog
pingpong
pingpong
pingpong
```

The first two `run` cases should report status 0; the missing program should report status 1. Each `pingpong` should print the child's `received ping` before the parent's `received pong`. PIDs vary. These commands belong in the xv6 shell; `make qemu` belongs in Ubuntu.

## Reconstruction

The PDF is unchanged; Markdown and DOCX are text reconstructions. The printed C excerpts were completed with includes, entry points and error checks. `run [command args...]` reproduces the report's source-edit variants without modifying the program. The five required observation answers are included in `notes.md`, completing the separate notes mentioned in the PDF. The optional `primes` task is not reported as completed and has not been added.

See [repository validation](../IMPLEMENTATION.md) for checks performed on the host. Host compilation does not replace an xv6/QEMU runtime test.

## Automated QEMU run

The [shared runner](../scripts/test_xv6.py) builds the actual xv6 kernel and runs the echo, ls and failed-exec variants followed by three ping/pong exchanges. It requires the documented pinned source revision and works in a temporary copy. [GitHub Actions](https://github.com/AlexxxCP/Operating-Systems/actions/workflows/labs.yml) saves build and console logs for these checks.
