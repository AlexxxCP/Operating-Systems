# Lab 2 observations

These answers are based on the supplied PDF. The updated implementation and new automated checks are identified separately below.

## Part 2: Using xv6

1. Three included programs are `ls`, `cat` and `echo`. Other examples in the recorded directory listing are `grep`, `wc`, `mkdir` and `sh`.
2. `ls | grep c` needs separate processes and a pipe. The shell creates the children with `fork`, connects output to input with `pipe` and descriptor duplication, and starts the programs with `exec`. The kernel transfers the bytes and synchronizes readers and writers.
3. Both the Linux and xv6 shells run programs and support pipes and redirection. xv6 supplies a much smaller set of commands. Unlike Bash, its shell does not implement globbing, variable expansion, quote processing or a PATH search; these differences are explored in Lab 4.

## Part 3: Reading the source

1. `user/cat.c` reads with `read` and sends bytes to descriptor 1 using `write`. It also uses `open` and `close` for named files and `exit` to finish. The program requests kernel services rather than accessing devices directly.
2. The report located `sys_read` and `sys_write` in `kernel/sysfile.c` at lines 69 and 83 in the VM's source tree. Line numbers are revision-specific. To locate them in another checkout, run `grep -n -E 'sys_read|sys_write' kernel/sysfile.c`.
3. `kernel/` implements the privileged operating system. Programs in `user/` run in user mode and request services through system calls. The declarations in `user/user.h`, generated stubs in `user/usys.S`, the dispatch table in `kernel/syscall.c` and the handlers in `kernel/sysfile.c` connect the two sides.

## Part 4: sleep

The executable is named `sleep`, but the waiting syscall in the documented xv6 revision is `pause(int)`. `user/user.h` declares `pause`, `user/usys.pl` generates its stub and `kernel/sysproc.c` implements `sys_pause`. Renaming the C call to `sleep` without changing the source revision would break the build.

The argument is a count of **clock ticks**, not seconds. Adding `$U/_sleep` to `UPROGS` causes the program to be built and included in `fs.img`; merely placing `sleep.c` in `user/` does not install it.

The original report demonstrated `sleep 10` and the usage error for missing arguments. The reviewed repository version additionally validates decimal input, negative values and integer overflow, and checks the syscall result. It accepts zero and leading zeros; it rejects signs, whitespace, non-digits, empty strings, too many arguments and values above 2147483647. The original PDF retains the earlier `atoi` implementation as a record of that session.
