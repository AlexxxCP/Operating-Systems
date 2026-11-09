"""Build and run Labs 2, 8 and 10 in a disposable copy of a pinned xv6 tree.

Requires Linux, make, RISC-V GCC/binutils and qemu-system-riscv64.
Usage: python3 scripts/test_xv6.py --xv6 /path/to/base-xv6 --output /path/to/logs
"""
from pathlib import Path
import argparse
import os
import re
import select
import shutil
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]
REVISION = 'f780d7850765d74cbdcd1fff4b822d8829f7c9bd'


class Console:
    def __init__(self, source, log):
        self.log = log
        self.pending = b''
        self.process = subprocess.Popen([
            'qemu-system-riscv64', '-machine', 'virt', '-bios', 'none',
            '-kernel', 'kernel/kernel', '-m', '128M', '-smp', '1', '-nographic',
            '-global', 'virtio-mmio.force-legacy=false',
            '-drive', 'file=fs.img,if=none,format=raw,id=x0',
            '-device', 'virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0',
        ], cwd=source, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    def prompt(self, timeout=60):
        deadline = time.monotonic() + timeout
        while True:
            match = re.search(br'(?:^|\r?\n)\$ ', self.pending)
            if match:
                data = self.pending[:match.end()]
                self.pending = self.pending[match.end():]
                return data.decode('utf-8', errors='replace').replace('\r', '')
            if time.monotonic() >= deadline:
                raise TimeoutError('xv6 prompt timeout; see qemu.txt for console output')
            readable, _, _ = select.select([self.process.stdout], [], [], 0.25)
            if readable:
                data = os.read(self.process.stdout.fileno(), 65536)
                if not data:
                    raise RuntimeError('QEMU exited before an xv6 prompt')
                self.pending += data
                self.log.write(data)
                self.log.flush()

    def command(self, line, timeout=60):
        print('xv6> '+line, flush=True)
        self.process.stdin.write((line+'\n').encode())
        self.process.stdin.flush()
        return self.prompt(timeout)

    def close(self):
        if self.process.poll() is None:
            try:
                self.process.stdin.write(b'\x01x')
                self.process.stdin.flush()
                self.process.wait(timeout=5)
            except (BrokenPipeError, subprocess.TimeoutExpired):
                self.process.kill()
                self.process.wait(timeout=5)
        self.process.stdin.close()
        self.process.stdout.close()


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def traces(output):
    return re.findall(r'(\d+): syscall (\w+) -> (-?\d+)', output)


def checks(console, readme_size):
    output = console.command('sleeptest')
    require('sleeptest: ALL TESTS PASSED' in output, 'Lab 2 status/timing tests failed')
    print('PASS Lab 2: valid/invalid arguments, exit statuses and actual elapsed ticks.', flush=True)

    output = console.command('run')
    require('hello from the child' in output and 'ended with status 0' in output, 'Lab 8 echo exec')
    output = console.command('run ls')
    require('README' in output and 'ended with status 0' in output, 'Lab 8 ls exec')
    output = console.command('run nosuchprog')
    require('exec failed' in output and 'ended with status 1' in output, 'Lab 8 failed exec status')
    for _ in range(3):
        output = console.command('pingpong')
        messages = re.findall(r'(\d+): received (ping|pong)', output)
        require(len(messages) == 2 and messages[0][1] == 'ping' and messages[1][1] == 'pong'
                and messages[0][0] != messages[1][0], 'Lab 8 ping/pong ordering and PIDs')
    print('PASS Lab 8: echo/ls/failed exec and three ordered ping/pong exchanges.', flush=True)

    output = console.command('trace 0 echo hi')
    require(not traces(output) and '\nhi\n' in output, 'Lab 10 zero mask')
    output = console.command('trace 32 grep hello README')
    calls = traces(output)
    require(bool(calls) and {call[1] for call in calls} == {'read'}, 'Lab 10 read-only mask')
    sizes = [int(call[2]) for call in calls]
    require(all(n >= 0 for n in sizes) and sum(sizes) == readme_size and sizes[-1] == 0,
            'Lab 10 read totals and EOF')
    output = console.command('trace 2147483647 grep hello README')
    names = {call[1] for call in traces(output)}
    require({'trace', 'exec', 'open', 'read', 'close'} <= names, 'Lab 10 all-call mask')
    output = console.command('trace 65536 echo hi')
    calls = traces(output)
    require(bool(calls) and {call[1] for call in calls} == {'write'}
            and sum(int(call[2]) for call in calls) == 3, 'Lab 10 write-only mask')
    output = console.command('trace 2129920 grep hello README')
    require({call[1] for call in traces(output)} == {'open', 'close'}, 'Lab 10 open/close mask')
    for command in ['trace abc echo rejected', 'trace -1 echo rejected',
                    'trace 2147483648 echo rejected']:
        output = console.command(command)
        require('mask must be an integer' in output and not re.search(r'^rejected$', output, re.M),
                'Lab 10 invalid mask executed command')
    output = console.command('trace')
    require('usage: trace mask command' in output, 'Lab 10 usage')
    output = console.command('trace 32 nosuchprog')
    require('trace: exec nosuchprog failed' in output, 'Lab 10 failed exec')
    output = console.command('tracecheck')
    require('tracecheck: ALL TESTS PASSED' in output, 'Lab 10 inheritance helper')
    parent = re.search(r'tracecheck: parent (\d+)', output).group(1)
    child = re.search(r'tracecheck: child (\d+)', output).group(1)
    calls = traces(output)
    require(len(calls) == 2 and {call[0] for call in calls} == {parent, child}
            and {call[1] for call in calls} == {'write'} and 'child-disabled' in output,
            'Lab 10 inherited/disabled masks')
    output = console.command('trace 2 usertests forkforkfork', timeout=120)
    calls = traces(output)
    require('ALL TESTS PASSED' in output and len({call[0] for call in calls}) > 1
            and {call[1] for call in calls} == {'fork'}, 'Lab 10 fork stress and inheritance')
    output = console.command('echo untraced')
    require('\nuntraced\n' in output and not traces(output), 'Lab 10 shell isolation after stress')
    print('PASS Lab 10: all masks, read totals/EOF, invalid input, fork stress, inheritance and isolation.', flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--xv6', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    revision = subprocess.check_output(['git', '-C', str(args.xv6), 'rev-parse', 'HEAD'], text=True).strip()
    require(revision == REVISION, 'Use the documented pinned xv6 revision: '+REVISION)
    require(not subprocess.check_output(['git', '-C', str(args.xv6), 'status', '--porcelain']),
            'The source xv6 checkout must be clean; tests only modify a temporary copy.')
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='os-labs-qemu-') as name:
        source = Path(name)/'xv6'
        shutil.copytree(args.xv6, source, ignore=shutil.ignore_patterns('.git'))
        patch = ROOT/'lab10/patches/trace.patch'
        subprocess.run(['git', '-c', 'core.autocrlf=false', 'apply', '--check', str(patch)], cwd=source, check=True)
        subprocess.run(['git', '-c', 'core.autocrlf=false', 'apply', str(patch)], cwd=source, check=True)
        files = {'lab2/user/sleep.c': 'sleep.c', 'lab2/tests/sleeptest.c': 'sleeptest.c',
                 'lab8/user/run.c': 'run.c', 'lab8/user/pingpong.c': 'pingpong.c',
                 'lab10/tests/tracecheck.c': 'tracecheck.c'}
        for original, target in files.items():
            shutil.copyfile(ROOT/original, source/'user'/target)
        makefile = source/'Makefile'
        text = makefile.read_text()
        require(text.count('\nfs.img:') == 1, 'Cannot locate xv6 filesystem build rule')
        # Prerequisite lists expand while Make reads them, before recipes run.
        text = text.replace('\nfs.img:', '\nUPROGS += $U/_sleep $U/_sleeptest $U/_run $U/_pingpong $U/_tracecheck\n\nfs.img:')
        makefile.write_text(text)
        with (output/'build.txt').open('w') as build:
            subprocess.run(['make', '-j2', 'TOOLPREFIX=riscv64-linux-gnu-', 'kernel/kernel', 'fs.img'],
                           cwd=source, stdout=build, stderr=subprocess.STDOUT, check=True, timeout=180)
        print('PASS: full RISC-V kernel and user-program build.', flush=True)
        with (output/'qemu.txt').open('wb') as log:
            console = Console(source, log)
            try:
                console.prompt()
                checks(console, (source/'README').stat().st_size)
            finally:
                console.close()
    (output/'result.txt').write_text('PASS: full RISC-V build and Linux/QEMU runtime checks for Labs 2, 8 and 10.\n')


if __name__ == '__main__':
    main()
