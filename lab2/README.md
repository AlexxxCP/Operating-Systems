# Lab 2: Meet the OS You Will Build

[Original PDF](Lab2_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

The PDF is preserved byte for byte. Markdown and DOCX were reconstructed from its text; layout, tables and spacing can differ. There are no embedded screenshot images in the supplied PDF.

## Implementation files

- [user/sleep.c](user/sleep.c)
- [Makefile.inc](Makefile.inc)

## Running the experiments

Copy `user/sleep.c` into the matching xv6-riscv checkout. Add `$U/_sleep` to its existing `UPROGS` list (or include `Makefile.inc` after the definition). Run `make qemu`; inside xv6, try `sleep 10` and `sleep`.

The report's xv6 version declares `pause(int)`. Check `user/user.h`; older versions that declare `sleep(int)` need the corresponding call instead. The fragment is an addition to the upstream Makefile, not a complete xv6 build configuration.

## Source

Luchiciov Alexei, FAF-241. Verified by Reiţman Patricia, Technical University of Moldova, 2026. Implementation files were transcribed or reconstructed from this lab's PDF; reconstructed additions are documented above.
