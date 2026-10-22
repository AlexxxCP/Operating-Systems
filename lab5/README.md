# Lab 5: The File System with Your Own Hands

[Original PDF](Lab5_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

The PDF is preserved byte for byte. Markdown and DOCX were reconstructed from its text; layout, tables and spacing can differ. There are no embedded screenshot images in the supplied PDF.

## Implementation files

- [commands.sh](commands.sh)

## Running the experiments

Run `bash commands.sh` in Linux as an ordinary user. The script creates a new temporary directory for the hard-link, symbolic-link, sparse-file and permission experiments, and prints its location. Expected permission failures are allowed to continue. The 1 GiB file is sparse. Protected password-file inspection is limited to metadata.

These commands are reconstructed from the report and adapted to use a fresh directory. File sizes, inode numbers, ownership, permissions and device names depend on the host.

## Source

Luchiciov Alexei, FAF-241. Verified by Reiţman Patricia, Technical University of Moldova, 2026. Implementation files were transcribed or reconstructed from this lab's PDF; reconstructed additions are documented above.
