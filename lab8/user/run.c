/* Reconstructed from the Lab 8 excerpt. Optional command arguments make the
 * echo/ls/failed-exec variants repeatable without source edits. */
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("parent: my pid is %d\n", getpid());
  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  }
  if(pid == 0){
    printf("child: my pid is %d\n", getpid());
    char *default_args[] = {"echo", "hello", "from", "the", "child", 0};
    char **args = argc > 1 ? argv + 1 : default_args;
    exec(args[0], args);
    fprintf(2, "exec failed\n");
    exit(1);
  }
  int status;
  if(wait(&status) < 0){
    fprintf(2, "wait failed\n");
    exit(1);
  }
  printf("parent: child %d ended with status %d\n", pid, status);
  exit(status);
}
