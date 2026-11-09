/* Reconstructed from the Lab 7 excerpt. Optional command selects the PDF's
 * ls/date/missing-command experiments without editing the source each time. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc > 2) {
        fprintf(stderr, "usage: run [command]\n");
        return 1;
    }
    const char *command = argc == 2 ? argv[1] : "ls";
    printf("parent: my PID is %ld\n", (long)getpid());
    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        printf("child: my PID is %ld, my parent is %ld\n",
               (long)getpid(), (long)getppid());
        fflush(stdout);
        if (strcmp(command, "ls") == 0)
            execlp(command, command, "-l", (char *)NULL);
        else
            execlp(command, command, (char *)NULL);
        perror("exec");
        _exit(127);
    }
    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) continue;
        perror("waitpid");
        return 1;
    }
    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        printf("parent: child %ld ended with status %d\n", (long)pid, code);
        return code;
    }
    fprintf(stderr, "parent: child terminated by signal %d\n", WTERMSIG(status));
    return 1;
}
