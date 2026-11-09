/* Reconstructed from Lab 7: delay waitpid() for 20 seconds. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    printf("parent: my PID is %ld\n", (long)getpid());
    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        printf("child: my PID is %ld, my parent is %ld\n",
               (long)getpid(), (long)getppid());
        fflush(stdout);
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("exec");
        _exit(127);
    }
    unsigned int remaining = 20;
    while (remaining != 0) remaining = sleep(remaining);
    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) continue;
        perror("waitpid");
        return 1;
    }
    if (!WIFEXITED(status)) return 1;
    printf("parent: child %ld ended with status %d\n",
           (long)pid, WEXITSTATUS(status));
    return WEXITSTATUS(status);
}
