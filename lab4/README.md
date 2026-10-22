# Lab 4: The xv6 Shell from the Outside

[Original PDF](Lab4_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

The PDF is preserved byte for byte. Markdown and DOCX were reconstructed from its text; layout, tables and spacing can differ. There are no embedded screenshot images in the supplied PDF.

## Implementation files

- [user/sleep.c](user/sleep.c)
- [user/args.c](user/args.c)
- [args_linux.c](args_linux.c)
- [Makefile.inc](Makefile.inc)
- [xv6-commands.txt](xv6-commands.txt)

## Running the experiments

Copy the files in `user/` to the matching xv6 checkout. Add `$U/_sleep` and `$U/_args` to `UPROGS`, using `Makefile.inc` as a fragment. If sleep is already listed from Lab 2, keep a single entry. Build with `make qemu`; `xv6-commands.txt` lists interactive comparisons from the report.

For the Linux comparison:

```bash
gcc -std=c99 -Wall -Wextra -o args args_linux.c
./args *.c "x y"
```

`args_linux.c` is reconstructed from the described implementation and printed output. The number of arguments after globbing depends on which C files are in the current directory. The xv6 `sleep` program uses `pause()`, as in Lab 2.

## Source

Luchiciov Alexei, FAF-241. Verified by Reiţman Patricia, Technical University of Moldova, 2026. Implementation files were transcribed or reconstructed from this lab's PDF; reconstructed additions are documented above.
