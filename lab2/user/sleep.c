/* Reviewed Lab 2 implementation. This xv6 revision uses pause(int). */
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }
  // atoi() cannot distinguish invalid input from zero or detect overflow.
  int ticks = 0;
  if(argv[1][0] == '\0'){
    fprintf(2, "sleep: ticks must be a non-negative integer\n");
    exit(1);
  }
  for(char *p = argv[1]; *p; p++){
    if(*p < '0' || *p > '9' || ticks > (2147483647 - (*p - '0')) / 10){
      fprintf(2, "sleep: ticks must be an integer from 0 to 2147483647\n");
      exit(1);
    }
    ticks = ticks * 10 + (*p - '0');
  }
  if(pause(ticks) < 0){
    fprintf(2, "sleep: pause failed\n");
    exit(1);
  }
  exit(0);
}
