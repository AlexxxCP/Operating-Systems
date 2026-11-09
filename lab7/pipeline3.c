/* Reconstructed from the optional Lab 7 excerpt: ls /usr/bin | grep x | wc -l. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    int p1[2], p2[2];
    if (pipe(p1) < 0) { perror("pipe"); return 1; }
    if (pipe(p2) < 0) { perror("pipe"); close(p1[0]); close(p1[1]); return 1; }
    pid_t children[3];
    int spawned = 0, failed = 0;
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); failed = 1; break; }
        if (pid == 0) {
            if ((i == 0 && dup2(p1[1], STDOUT_FILENO) < 0) ||
                (i == 1 && (dup2(p1[0], STDIN_FILENO) < 0 || dup2(p2[1], STDOUT_FILENO) < 0)) ||
                (i == 2 && dup2(p2[0], STDIN_FILENO) < 0)) {
                perror("dup2"); _exit(1);
            }
            close(p1[0]); close(p1[1]); close(p2[0]); close(p2[1]);
            if (i == 0) execlp("ls", "ls", "/usr/bin", (char *)NULL);
            else if (i == 1) execlp("grep", "grep", "x", (char *)NULL);
            else execlp("wc", "wc", "-l", (char *)NULL);
            perror("exec");
            _exit(127);
        }
        children[spawned++] = pid;
    }
    close(p1[0]); close(p1[1]); close(p2[0]); close(p2[1]);
    for (int i = 0; i < spawned; i++) {
        int status;
        pid_t result;
        do { result = waitpid(children[i], &status, 0); } while (result < 0 && errno == EINTR);
        if (result < 0 || !WIFEXITED(status)) failed = 1;
        else if (WEXITSTATUS(status) != 0 && !(i == 1 && WEXITSTATUS(status) == 1)) failed = 1;
    }
    return failed;
}
