/* Runtime check: inherited writes, disabling tracing and exec retention. */
#include "kernel/types.h"
#include "kernel/syscall.h"
#include "user/user.h"

int
main(void)
{
  printf("tracecheck: parent %d\n", getpid());
  if(trace(1 << SYS_write) < 0) exit(1);
  if(write(1, "parent marker\n", 14) != 14) exit(1);
  int pid = fork();
  if(pid < 0) exit(1);
  if(pid == 0){
    if(write(1, "child marker\n", 13) != 13) exit(1);
    if(trace(0) < 0) exit(1);
    char *args[] = {"echo", "child-disabled", 0};
    exec("echo", args);
    exit(1);
  }
  int status;
  if(wait(&status) < 0 || status != 0) exit(1);
  if(trace(0) < 0) exit(1);
  printf("tracecheck: child %d\n", pid);
  printf("tracecheck: ALL TESTS PASSED\n");
  exit(0);
}
