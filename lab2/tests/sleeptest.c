/* Run inside xv6. Each exec uses the actual installed sleep binary. */
#include "kernel/types.h"
#include "user/user.h"

static int
check(char **args, int expected, int minimum_ticks)
{
  int start = uptime();
  int pid = fork();
  if(pid < 0){
    fprintf(2, "sleeptest: fork failed\n");
    return 1;
  }
  if(pid == 0){
    exec("sleep", args);
    fprintf(2, "sleeptest: cannot exec sleep\n");
    exit(99);
  }
  int status;
  if(wait(&status) < 0 || status != expected || uptime() - start < minimum_ticks){
    fprintf(2, "sleeptest: status or timing mismatch\n");
    return 1;
  }
  return 0;
}

int
main(void)
{
  char *valid[] = {"sleep", "5", 0};
  char *zero[] = {"sleep", "0", 0};
  char *leading_zero[] = {"sleep", "001", 0};
  char *missing[] = {"sleep", 0};
  char *extra[] = {"sleep", "1", "2", 0};
  char *invalid[] = {"sleep", "abc", 0};
  char *negative[] = {"sleep", "-1", 0};
  char *overflow[] = {"sleep", "2147483648", 0};
  char *empty[] = {"sleep", "", 0};
  char *space[] = {"sleep", " 1", 0};
  int failed = check(valid, 0, 5) | check(zero, 0, 0) | check(leading_zero, 0, 1);
  failed |= check(missing, 1, 0) | check(extra, 1, 0) | check(invalid, 1, 0);
  failed |= check(negative, 1, 0) | check(overflow, 1, 0) | check(empty, 1, 0) | check(space, 1, 0);
  if(failed) exit(1);
  printf("sleeptest: ALL TESTS PASSED\n");
  exit(0);
}
