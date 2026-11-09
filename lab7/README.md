# Lab 7: Three Small Programs: Processes, Redirection, a Pipe

[Original PDF](Lab7_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Read on GitHub](report.md) · [Editable DOCX](report.docx)

Linux process creation, child exit statuses, zombies, output redirection and pipelines.

## Files

| File | Purpose |
| --- | --- |
| [run.c](run.c) | `fork`, `execlp` and `waitpid`; defaults to `ls -l`. |
| [zombie.c](zombie.c) | Waits 20 seconds before reaping the child. |
| [redirect.c](redirect.c) | Replaces stdout with `out.txt` using `close` and `open`. |
| [pipeline.c](pipeline.c) | `ls /etc \| wc -l`. |
| [pipeline3.c](pipeline3.c) | Optional three-stage pipeline: `ls /usr/bin \| grep x \| wc -l`. |
| [Makefile](Makefile) | Linux build targets. |
| [tests/test.sh](tests/test.sh) | Builds and checks all five programs in a temporary directory. |
| [notes.md](notes.md) | Observations and recorded results from the PDF. |

## Build and run in Ubuntu

```bash
make
./run
./run date
./run nosuchprog
./redirect
head out.txt
./pipeline
./pipeline3
make test
```

The test takes about 20 seconds because it checks the zombie before and after `waitpid`. It uses Linux `/proc`, `ps` and `timeout`. It compares pipeline counts with commands on the current machine; 236 and 229 are historical PDF results, not fixed expected counts.

To inspect the zombie manually, run `./zombie > /dev/null &` followed by `ps -o pid,ppid,stat,cmd`. The child's state should be `Z` during the 20-second delay.

## Reconstruction

The original PDF is preserved byte for byte. DOCX and Markdown were extracted from its text, so layout can differ. Complete source files were reconstructed from the printed excerpts. Includes, entry points, error handling, descriptor cleanup and child-status checks were added. The default operations match the report; `run [command]` makes the report's modified-command experiments repeatable. Pipeline descriptor duplication uses `dup2`. The intentionally broken pipeline from the PDF is described in the notes; the distributed implementation closes all unused ends.

See [repository validation](../IMPLEMENTATION.md) for checks performed on the host and the limits of those checks.
