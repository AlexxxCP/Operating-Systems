# Lab 6: Inside the xv6 File System

[Original PDF](Lab6_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

The PDF is preserved byte for byte. Markdown and DOCX were reconstructed from its text; layout, tables and spacing can differ. There are no embedded screenshot images in the supplied PDF.

## Implementation files

- [user/mystat.c](user/mystat.c)
- [user/find.c](user/find.c)
- [Makefile.inc](Makefile.inc)
- [xv6-commands.txt](xv6-commands.txt)

## Running the experiments

The PDF prints excerpts of `mystat` and `find`. The files in `user/` complete those excerpts with headers, entry points, argument checks, bounds checks and error handling. They are reconstructed implementations.

Copy them into the matching xv6 checkout, add `$U/_mystat` and `$U/_find` to `UPROGS` (see `Makefile.inc`), then run `make qemu`. Enter the examples in `xv6-commands.txt` interactively. The hard-link example uses a disposable file. The original captured results and unexpected exec failures remain in the PDF and its converted versions.

## Source

Luchiciov Alexei, FAF-241. Verified by Reiţman Patricia, Technical University of Moldova, 2026. Implementation files were transcribed or reconstructed from this lab's PDF; reconstructed additions are documented above.
