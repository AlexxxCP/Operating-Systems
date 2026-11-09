# Lab 8 observations

Answers reconstructed from the supplied PDF.

## Part 1: run

1. xv6 uses `fork`, `exec` and `wait`; Linux Lab 7 used `fork`, `execlp` and `waitpid`. Both create a child, replace its image and collect its termination status. Their interfaces and libraries differ.
2. The `echo` executable prints `hello from the child`. A successful `exec` replaces the original child program.
3. The report's successful `echo` and `ls` variants returned status 0. Failed `exec` returned status 1 because this program explicitly calls `exit(1)`; Lab 7 selected 127. The restored source used `echo`.

## Part 2: pingpong

1. One pipe carries the parent-to-child byte; the other carries the reply. Separate one-way streams avoid confusing the request with the response. Both processes close unused pipe endpoints.
2. The parent blocks on `read(c2p[0], &b, 1)` for the reply. The later `wait` reaps the child; it is not the operation that receives the byte.

The PDF records three successful message pairs: child/parent PIDs 4/3, 6/5 and 8/7. Those are historical results. New runs should preserve message order, with their own PIDs.

The initial `exec run failed` was caused by a missing `UPROGS` entry. Editing the Makefile and rebuilding included the program in `fs.img`.
