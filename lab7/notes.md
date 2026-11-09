# Lab 7 observations

Reconstructed from the supplied report; the following recorded numbers were not produced by a new run.

- A successful `execlp` replaces the child image and never reaches the next statement. A missing executable reaches `perror` and exits with the program-selected status 127. The parent uses `WEXITSTATUS` after checking `WIFEXITED`.
- In the report's first run, parent PID 4753 matched the child's PPID, and child PID 4755 terminated with status 0. PIDs vary between executions.
- Delaying `waitpid` left the completed `ls` child in state `Z` (`<defunct>`). Reaping the child removed that entry; a zombie is already finished, rather than a process still doing work.
- After closing descriptor 1, `open("out.txt", ...)` reused it. The `ls` listing went to the file, while the descriptor diagnostic was sent to stderr, descriptor 2.
- Both the C two-stage pipeline and `ls /etc | wc -l` printed 236 in the recorded Ubuntu session.
- Keeping the parent's pipe write end open prevented `wc` from seeing EOF. Restoring `close(p[1])` allowed it to terminate.
- The optional three-stage pipeline and its shell equivalent both printed 229 in the recorded session.
