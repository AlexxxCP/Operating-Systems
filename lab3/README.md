# Lab 3: The Command Line: Shell, Pipes and Text Filters

[Original PDF](Lab3_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

The PDF is preserved byte for byte. Markdown and DOCX were reconstructed from its text; layout, tables and spacing can differ. There are no embedded screenshot images in the supplied PDF.

## Implementation files

- [generate_access_log.sh](generate_access_log.sh)
- [oneliners.sh](oneliners.sh)
- [report.sh](report.sh)
- [data/access.log](data/access.log)
- [expected-report.txt](expected-report.txt)

## Running the experiments

Run from this directory:

```bash
bash report.sh data/access.log
bash oneliners.sh data/access.log
bash generate_access_log.sh new-access.log
```

`data/access.log` is regenerated from the exact 500-row formula in the PDF. `expected-report.txt` contains the recorded summary: 500 requests and 100 failures. The generator refuses to overwrite an existing output. `report.sh` also validates a missing argument. Q7 and Q8 inspect the current host and their values vary between systems.

## Source

Luchiciov Alexei, FAF-241. Verified by Reiţman Patricia, Technical University of Moldova, 2026. Implementation files were transcribed or reconstructed from this lab's PDF; reconstructed additions are documented above.
