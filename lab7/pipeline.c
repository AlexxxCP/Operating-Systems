/* Reconstructed from Lab 7: ls /etc | wc -l. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    int p[2];
    if (pipe(p) < 0) { perror("pipe"); return 1; }
    pid_t children[2];
    for (int i = 0; i < 2; i++) {
        children[i] = fork();
        if (children[i] < 0) {
            perror("fork");
            close(p[0]); close(p[1]);
            if (i != 0) while (waitpid(children[0], NULL, 0) < 0 && errno == EINTR) {}
            return 1;
        }
        if (children[i] == 0) {
            int fd = i == 0 ? p[1] : p[0];
            int target = i == 0 ? STDOUT_FILENO : STDIN_FILENO;
            if (dup2(fd, target) < 0) { perror("dup2"); _exit(1); }
            close(p[0]); close(p[1]);
            if (i == 0) execlp("ls", "ls", "/etc", (char *)NULL);
            else execlp("wc", "wc", "-l", (char *)NULL);
            perror("exec");
            _exit(127);
        }
    }
    close(p[0]); close(p[1]);
    int failed = 0;
    for (int i = 0; i < 2; i++) {
        int status;
        pid_t result;
        do { result = waitpid(children[i], &status, 0); } while (result < 0 && errno == EINTR);
        if (result < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) failed = 1;
    }
    return failed;
}
