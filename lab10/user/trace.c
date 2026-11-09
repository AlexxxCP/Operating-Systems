/* Reviewed wrapper based on Lab 10, including decimal-mask validation. */
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: trace mask command [args...]\n");
    exit(1);
  }
  int mask = 0;
  if(argv[1][0] == '\0'){
    fprintf(2, "trace: mask must be an integer from 0 to 2147483647\n");
    exit(1);
  }
  for(char *p = argv[1]; *p; p++){
    if(*p < '0' || *p > '9' || mask > (2147483647 - (*p - '0')) / 10){
      fprintf(2, "trace: mask must be an integer from 0 to 2147483647\n");
      exit(1);
    }
    mask = mask * 10 + (*p - '0');
  }
  if(trace(mask) < 0){
    fprintf(2, "trace: failed\n");
    exit(1);
  }
  exec(argv[2], argv + 2);
  fprintf(2, "trace: exec %s failed\n", argv[2]);
  exit(1);
}
