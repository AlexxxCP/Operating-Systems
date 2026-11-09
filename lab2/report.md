# Laboratory Work No. 2: Meet the OS You Will Build

**Operating Systems — Technical University of Moldova**

Student: Luchiciov Alexei, FAF-241. Verified by: Reiţman Patricia.

[Original PDF](Lab2_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Implementation](user/sleep.c) · [Observe answers](notes.md)

This readable edition is based on the original PDF. Recorded command results below belong to the original VM session. The reviewed implementation and additional tests are described separately; they do not change the historical PDF.

## 1. Introduction and environment

The work introduces xv6, a small Unix-like teaching operating system. Its source was compiled in Ubuntu and booted in the QEMU RISC-V emulator. The experiments used xv6 commands, inspected system-call implementations and added the `sleep` user program.

The VM used the source directory `~/xv6-riscv` and reported QEMU 10.2.1. The repository's repeatable checks use [official xv6 revision f780d785](https://github.com/mit-pdos/xv6-riscv/tree/f780d7850765d74cbdcd1fff4b822d8829f7c9bd), with `pause(int)` declared in `user/user.h`.

## 2. Build and boot

Commands executed in Ubuntu:

~~~bash
cd ~/xv6-riscv
ls
qemu-system-riscv64 --version
make qemu
~~~

The project contained `kernel/`, `user/`, `mkfs/`, `Makefile` and `README`. The recorded boot output was:

~~~text
xv6 kernel is booting
hart 1 starting
hart 2 starting
init: starting sh
$
~~~

The initial process starts the shell; its prompt indicates that user commands can run. A hart is a RISC-V hardware thread. The number of hart messages depends on QEMU's configured CPU count.

## 3. Using xv6 as Unix

Commands entered at the xv6 prompt:

~~~text
ls
cat README
echo hello xv6
ls | grep c
wc README
~~~

The recorded directory listing included ls, cat, echo, grep, wc, mkdir and sh. Three examples of shipped programs are therefore ls, cat and echo.

The echo and word-count results were:

~~~text
$ echo hello xv6
hello xv6
$ wc README
48 336 2441 README
~~~

The recorded pipe output included:

~~~text
cat 2 3 36728
echo 2 4 35592
wc 2 17 37728
sync 2 22 34944
console 3 23 0
~~~

The shell starts separate programs and connects them with a kernel pipe: the first process's output becomes the second process's input. fork, exec, pipe, descriptor duplication and waiting provide the necessary process and communication operations.

Both Bash and the xv6 shell support program execution, pipes and redirection. xv6 is deliberately smaller: Bash's globbing, variable expansion, quoting, PATH lookup, history and completion are not all implemented in its shell. Lab 4 explores these differences.

The README counts, executable sizes and inode numbers above are recorded observations, not portable expected values.

## 4. User programs and kernel services

After leaving QEMU, the following source was inspected in Ubuntu:

~~~bash
sed -n '1,40p' user/ls.c
sed -n '1,30p' user/cat.c
grep -n -E 'sys_read|sys_write' kernel/sysfile.c
~~~

cat uses read to obtain bytes and write to send them to stdout, descriptor 1. It also uses open, close and exit:

~~~c
while ((n = read(fd, buf, sizeof(buf))) > 0) {
  if (write(1, buf, n) != n) {
    fprintf(2, "cat: write error\n");
    exit(1);
  }
}
~~~

In the original source checkout, the search located sys_read at line 69 and sys_write at line 83 of kernel/sysfile.c. The file and function names identify the implementations; line numbers can move between revisions.

user/ contains ordinary programs running in user mode. kernel/ implements the privileged operating system. A user declaration in user/user.h leads to a generated stub in user/usys.S, then a system-call trap, dispatch through kernel/syscall.c and the corresponding kernel handler. User programs obtain services through this interface rather than executing privileged kernel code directly.

## 5. The sleep program

### 5.1. Version compatibility and units

The executable is called sleep, but the waiting system call in this xv6 version is called pause. The original build failed when it called an undeclared sleep function; changing the call to pause matched user/user.h.

The argument counts **clock ticks**, not seconds. Check the declarations in the actual xv6 checkout before adapting the program to a different revision.

### 5.2. Original recorded implementation and tests

The original PDF's implementation checked argc == 2, called pause(atoi(argv[1])) and exited with status 0. Adding $U/_sleep to UPROGS caused it to be compiled, linked and included in fs.img.

The recorded build commands were make clean and make qemu. The VM then showed the sleep executable in ls and ran:

~~~text
$ sleep 10
$
$ sleep
usage: sleep <ticks>
$
~~~

Those observations demonstrated the normal and missing-argument cases. They did not test non-numeric input, negative numbers, overflow or the waiting syscall's failure result.

### 5.3. Reviewed repository implementation

The maintained [user/sleep.c](user/sleep.c) additionally:

- accepts zero and non-negative decimal tick counts, including leading zeros;
- rejects missing/extra arguments, empty strings, signs, whitespace and non-digits;
- checks the signed 32-bit limit before multiplying or adding, avoiding integer overflow;
- exits unsuccessfully if pause fails, rather than reporting success unconditionally.

The original PDF remains the record of the earlier implementation. The source file is the current reviewed implementation.

### 5.4. Build integration

Copy user/sleep.c into the matching xv6 user/ directory. Add $U/_sleep to UPROGS, or include [Makefile.inc](Makefile.inc) immediately after the list definition and **before** the fs.img rule. Make expands target prerequisites while reading the Makefile, so adding programs only at its end can leave them unbuilt when fs.img is created.

make qemu is an Ubuntu build command. sleep 10 is entered inside the xv6 shell.

## 6. Additional reproducible checks

[tests/test_sleep_host.py](tests/test_sleep_host.py) compiles the actual C implementation with a stub waiting syscall. It checks the upper bound, invalid inputs, the forwarded tick value and propagation of a syscall failure. This does not simulate real elapsed ticks.

[tests/sleeptest.c](tests/sleeptest.c) runs inside xv6, executes the installed sleep binary and checks child exit statuses. It measures uptime() around a five-tick wait and requires at least five elapsed ticks. It also covers zero, leading zeros, missing/extra arguments, invalid text, negative values, overflow, empty strings and leading whitespace.

The [shared QEMU runner](../scripts/test_xv6.py) installs the helper in a disposable copy of the pinned xv6 source, builds the real RISC-V kernel and user programs, and runs these tests. [GitHub Actions](https://github.com/AlexxxCP/Operating-Systems/actions/workflows/labs.yml) retains its build and console logs. Consult the run status and [validation notes](../IMPLEMENTATION.md) for the actual results.

## 7. Conclusion

xv6 provides familiar Unix concepts with a small codebase: ordinary commands use system calls, pipes connect processes, and user and kernel code remain distinct. Adding sleep demonstrated the path from a C source file through UPROGS and fs.img to execution inside the OS.

The original session confirmed a successful wait and the usage error. The reviewed repository makes the units and pause API explicit, completes the separate Observe answers and adds checks for input validation, failure handling and actual elapsed ticks.
