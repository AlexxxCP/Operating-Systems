# Lab 2: Meet the OS You Will Build

[Original PDF](Lab2_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

Build xv6, use its Unix programs, inspect the user/kernel boundary and install a clock-tick waiting program. The original PDF preserves the recorded session; Markdown and DOCX include the reviewed explanation and implementation changes.

## Implementation files

- [user/sleep.c](user/sleep.c)
- [Makefile.inc](Makefile.inc)
- [notes.md](notes.md): all Part 2 and Part 3 Observe answers and the `pause` version note.
- [xv6-commands.txt](xv6-commands.txt): commands and expected behavior.
- [tests/test_sleep_host.py](tests/test_sleep_host.py): input/error checks with a stubbed waiting syscall.
- [tests/sleeptest.c](tests/sleeptest.c): actual xv6 execution, exit-status and timing tests.

## Running the experiments

Use the documented [xv6 revision f780d785](https://github.com/mit-pdos/xv6-riscv/tree/f780d7850765d74cbdcd1fff4b822d8829f7c9bd). Copy `user/sleep.c` into its `user/` directory. Add `$U/_sleep` to the existing `UPROGS` list, or include `Makefile.inc` immediately after that definition and **before** the `fs.img` rule. Run `make qemu`; inside xv6, try `sleep 10`, `sleep 0`, `sleep`, `sleep abc` and `sleep -1`.

The report's xv6 version declares `pause(int)`. Check `user/user.h`; older versions that declare `sleep(int)` need the corresponding call instead. The fragment is an addition to the upstream Makefile, not a complete xv6 build configuration.

Ticks are not seconds. The reviewed source accepts decimal values from 0 to 2147483647, rejects invalid input and overflow, and reports a failed waiting syscall. The original PDF's `atoi` version is retained only as the historical listing.

## Reproducible checks

```bash
python3 lab2/tests/test_sleep_host.py
python3 scripts/test_xv6.py --xv6 /path/to/clean-pinned-xv6 --output /path/to/test-logs
```

Run these from the Operating-Systems repository root. The first requires Python and a host C compiler. The second requires Linux, RISC-V GCC/binutils and QEMU; it builds and tests Labs 2, 8 and 10 in a temporary copy, without modifying the base checkout. [GitHub Actions](https://github.com/AlexxxCP/Operating-Systems/actions/workflows/labs.yml) runs the same checks and retains build/console logs.

## Source

Luchiciov Alexei, FAF-241. Verified by Reiţman Patricia, Technical University of Moldova, 2026. Implementation files were transcribed or reconstructed from this lab's PDF; reconstructed additions are documented above.
