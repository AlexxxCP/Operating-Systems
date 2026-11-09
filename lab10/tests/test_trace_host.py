"""Host checks for the actual dispatcher and trace handler in a patched xv6 tree.

Usage: python3 test_trace_host.py /path/to/patched/xv6 [--cc gcc]
The mock process and syscall handlers do not emulate traps, fork, or RISC-V.
"""
from pathlib import Path
import argparse
import re
import subprocess
import tempfile


def function(text, declaration):
    start = text.index(declaration)
    opening = text.index('{', start)
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == '{':
            depth += 1
        elif text[index] == '}':
            depth -= 1
            if depth == 0:
                return text[start:index+1]
    raise ValueError('Unclosed function: '+declaration)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('xv6', type=Path)
    parser.add_argument('--cc', default='gcc')
    args = parser.parse_args()
    tree = args.xv6
    definitions = (tree/'kernel/syscall.h').read_text()
    numbers = dict((name, int(number)) for name, number in
                   re.findall(r'^#define SYS_(\w+)\s+(\d+)', definitions, re.M))
    expected = {'fork': 1, 'read': 5, 'open': 15, 'write': 16, 'close': 21, 'sync': 22, 'trace': 23}
    if any(numbers.get(name) != number for name, number in expected.items()):
        raise RuntimeError('This test targets the documented xv6 syscall numbering.')
    source = (tree/'kernel/syscall.c').read_text()
    table = source.split('static const char *syscallnames[] = {', 1)[1].split('};', 1)[0]
    names = dict(re.findall(r'\[SYS_(\w+)\]\s*=\s*"(\w+)"', table))
    if any(names.get(name) != name for name in numbers):
        raise RuntimeError('A registered syscall has a missing or incorrect printable name.')
    start = source.index('extern uint64 sys_fork(void);')
    end = source.index('\nvoid\nsyscall(void)', start)
    dispatcher = source[start:end] + function(source, 'void\nsyscall(void)')
    handler = function((tree/'kernel/sysproc.c').read_text(), 'uint64\nsys_trace(void)')
    prototypes = re.findall(r'extern uint64 (sys_\w+)\(void\);', dispatcher)
    stubs = '\n'.join('uint64 '+name+'(void) { return result; }'
                      for name in prototypes if name != 'sys_trace')
    harness = r'''
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int uint;
typedef unsigned long long uint64;
#define NELEM(x) (sizeof(x) / sizeof((x)[0]))
struct trapframe { uint64 a0, a7; };
struct proc { int pid, trace_mask; char name[16]; struct trapframe *trapframe; };
static struct trapframe frame;
static struct proc process = {42, 0, "host-test", &frame};
static uint64 result;
static int argument, lines;
static char output[256];
struct proc *myproc(void) { return &process; }
void argint(int n, int *value) { (void)n; *value = argument; }
int printk(char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int count = vsnprintf(output, sizeof(output), format, ap);
  va_end(ap);
  lines++;
  return count;
}
static void check(int ok, const char *label) {
  if (!ok) { fprintf(stderr, "FAIL: %s\n", label); exit(1); }
}
'''
    harness += definitions+'\n'+dispatcher+'\n'+handler+'\n'+stubs+'\n'
    harness += r'''
int main(void) {
  for (unsigned int n = 1; n < NELEM(syscalls); n++) {
    if (!syscalls[n] || n == SYS_trace) continue;
    process.trace_mask = (int)(1U << n);
    result = 42; frame.a7 = n; lines = 0; output[0] = 0;
    syscall();
    check(lines == 1 && frame.a0 == 42, "selected call and return value");
    char expected[256];
    snprintf(expected, sizeof(expected), "42: syscall %s -> 42\n", syscallnames[n]);
    check(strcmp(output, expected) == 0, "syscall name and PID");
    process.trace_mask = 0; lines = 0;
    syscall();
    check(lines == 0, "zero mask");
    process.trace_mask = 32; lines = 0;
    syscall();
    check(lines == (n == SYS_read), "read-only mask");
    process.trace_mask = 2129920; lines = 0;
    syscall();
    check(lines == (n == SYS_open || n == SYS_close), "open and close mask");
    process.trace_mask = 2147483647; lines = 0;
    syscall();
    check(lines == 1, "all-calls mask");
  }
  frame.a7 = SYS_trace; argument = 32; lines = 0;
  syscall();
  check(process.trace_mask == 32 && frame.a0 == 0 && lines == 0, "handler sets mask");
  argument = 1 << SYS_trace; lines = 0;
  syscall();
  check(lines == 1 && strstr(output, "syscall trace -> 0"), "trace can report itself");
  argument = 0; lines = 0;
  syscall();
  check(process.trace_mask == 0 && lines == 0, "disable trace");
  process.trace_mask = 2; frame.a7 = SYS_fork; result = (uint64)-1; lines = 0;
  syscall();
  check(lines == 1 && strstr(output, "syscall fork -> -1"), "negative return value");
  unsigned int invalid[] = {0, 31, 999};
  for (unsigned int i = 0; i < NELEM(invalid); i++) {
    frame.a7 = invalid[i]; lines = 0;
    syscall();
    check(frame.a0 == (uint64)-1 && lines == 1 && strstr(output, "unknown sys call"),
          "invalid syscall number");
  }
  puts("PASS: actual xv6 dispatcher/handler, selective masks, names, return values and invalid calls.");
  return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix='xv6-trace-test-') as directory:
        work = Path(directory)
        code = work/'trace_host.c'
        code.write_text(harness)
        executable = work/'trace_host.exe'
        # xv6's Makefile also suppresses sign-compare for num < NELEM(syscalls).
        subprocess.run([args.cc, '-std=c99', '-Wall', '-Wextra', '-Werror', '-Wno-sign-compare',
                        str(code), '-o', str(executable)], check=True)
        subprocess.run([str(executable)], check=True, timeout=15)


if __name__ == '__main__':
    main()
