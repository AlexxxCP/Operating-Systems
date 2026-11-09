# trace patch

Base: [mit-pdos/xv6-riscv, f780d7850765d74cbdcd1fff4b822d8829f7c9bd](https://github.com/mit-pdos/xv6-riscv/tree/f780d7850765d74cbdcd1fff4b822d8829f7c9bd). Upstream code is covered by [LICENSE.xv6](LICENSE.xv6).

This is a reconstructed patch, not a recovered diff from the original Ubuntu VM. Its syscall numbering and `printk`/`pause` interfaces match the supplied Lab 10 report. It changes:

| File in xv6 | Change |
| --- | --- |
| `kernel/syscall.h` | Register `SYS_trace = 23`, preserving `SYS_sync = 22`. |
| `kernel/proc.h` | Store an integer `trace_mask` per process. |
| `kernel/proc.c` | Initialize/reset the mask and copy it in `kfork` before making the child runnable. |
| `kernel/sysproc.c` | Read the mask with `argint` in `sys_trace`. |
| `kernel/syscall.c` | Register the handler, name every registered call and print selected returned results. |
| `user/user.h`, `user/usys.pl` | Declare the user API and generate its assembly stub. |
| `user/trace.c`, `Makefile` | Build the wrapper and include it in `fs.img`. |

The PDF shows the handler, inherited mask and conditional printing. The complete names table, patch context, initialization/reset and unsigned bounded mask selection were supplied during reconstruction. Existing syscall behavior is preserved.

The reviewed user wrapper validates the decimal mask and checks overflow before invoking `trace`. The original PDF preserves the earlier `atoi` listing; the patch and standalone source use the reviewed version.

Run `git apply --check` first. The patch already includes `user/trace.c` and its Makefile registration; copying the standalone file first would cause an add-file conflict.
