/* Reconstructed from Lab 7; close(1) + open() is intentionally retained. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        close(STDOUT_FILENO);
        int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) { perror("open"); _exit(1); }
        fprintf(stderr, "open gave descriptor %d\n", fd);
        if (fd != STDOUT_FILENO) {
            if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2"); _exit(1); }
            close(fd);
        }
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("exec");
        _exit(127);
    }
    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) continue;
        perror("waitpid");
        return 1;
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) return 1;
    puts("done: the listing is in out.txt");
    return 0;
}
