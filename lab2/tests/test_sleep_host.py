"""Check the actual sleep.c with a stubbed pause syscall; no xv6 timing is simulated."""
from pathlib import Path
import argparse
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='gcc')
    args = parser.parse_args()
    source = (Path(__file__).resolve().parents[1]/'user/sleep.c').read_text()
    source = '\n'.join(line for line in source.splitlines() if not line.startswith('#include'))
    harness = r'''
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
static jmp_buf finished;
static int status, calls, ticks, syscall_result, diagnostics;
static void mock_exit(int code) __attribute__((noreturn));
static void mock_exit(int code) { status = code; longjmp(finished, 1); }
static int mock_pause(int n) { calls++; ticks = n; return syscall_result; }
static void mock_fprintf(int fd, const char *fmt, ...) { (void)fd; (void)fmt; diagnostics++; }
#define exit mock_exit
#define pause mock_pause
#define fprintf mock_fprintf
#define main sleep_main
'''+source+r'''
#undef main
#undef fprintf
#undef pause
#undef exit
static void check(char *arg, int argc, int result, int expected_status, int expected_ticks) {
  char *argv[] = {"sleep", arg, "extra", NULL};
  calls = 0; ticks = -1; status = -1; diagnostics = 0; syscall_result = result;
  if (setjmp(finished) == 0) { sleep_main(argc, argv); abort(); }
  if (status != expected_status || calls != (expected_ticks >= 0) ||
      (calls && ticks != expected_ticks) || diagnostics != (expected_status != 0)) {
    fprintf(stderr, "FAIL: sleep argument [%s], argc %d\n", arg ? arg : "missing", argc);
    exit(1);
  }
}
int main(void) {
  check("0", 2, 0, 0, 0);
  check("10", 2, 0, 0, 10);
  check("0001", 2, 0, 0, 1);
  check("2147483647", 2, 0, 0, 2147483647);
  check(NULL, 1, 0, 1, -1);
  check("1", 3, 0, 1, -1);
  char *bad[] = {"", "abc", "-1", "+1", " 1", "1 ", "1x", "2147483648", "999999999999999999999"};
  for (unsigned int i = 0; i < sizeof(bad)/sizeof(bad[0]); i++) check(bad[i], 2, 0, 1, -1);
  check("5", 2, -1, 1, 5);
  puts("PASS: sleep argument validation, INT_MAX boundary, syscall argument and failure status.");
  return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix='sleep-host-test-') as name:
        work = Path(name)
        code = work/'sleep_host.c'
        code.write_text(harness)
        executable = work/'sleep_host.exe'
        subprocess.run([args.cc, '-std=c99', '-Wall', '-Wextra', '-Werror', str(code), '-o', str(executable)], check=True)
        subprocess.run([str(executable)], check=True, timeout=10)


if __name__ == '__main__':
    main()
