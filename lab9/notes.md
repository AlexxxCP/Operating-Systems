# Lab 9 observations

These values are taken from the supplied PDF, not from newly generated trace logs.

| Call in the saved `ls -l` trace | Count |
| --- | --- |
| `openat` | 31 |
| `read` | 20 |
| `write` | 1 |
| `close` | 34 |
| Total of these four types | 86 |

The independent `strace -c -e trace=openat` summary also reported 31 `openat` calls and one optional-locale-file `ENOENT`. The listing still exited successfully. Counts and timings vary between systems and runs.

`ls` sends its listing to descriptor 1, stdout, through `write`. In the saved trace one `write(1,...)` contained the listing. Creating output files before listing the directory explains why the later listing differed from the initially empty one.

Descriptors 0, 1 and 2 normally refer to stdin, stdout and stderr. The first file `open` returned the lowest available descriptor, 3; closing it allowed the read-only reopen to reuse 3. In that Linux/C-library version the C `open` wrapper appeared as the `openat` syscall.

The literal `hello, kernel\n` contains 14 bytes. The saved run wrote 14 bytes to descriptor 3, read the same 14 bytes back and created a 14-byte `note.txt`. Diagnostic `printf` messages go to stdout and are distinct from writes to the file.
