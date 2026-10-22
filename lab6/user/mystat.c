/*
 * Reconstructed from Lab 6. The printed helper and loop are retained;
 * includes, main(), argument validation and error status complete the excerpt.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static char *
typename(int t)
{
  if(t == T_DIR) return "directory";
  if(t == T_FILE) return "file";
  if(t == T_DEVICE) return "device";
  return "?";
}

int
main(int argc, char *argv[])
{
  struct stat st;
  int failed = 0;
  if(argc < 2){
    fprintf(2, "usage: mystat <path> [path ...]\n");
    exit(1);
  }
  for(int i = 1; i < argc; i++){
    if(stat(argv[i], &st) < 0){
      fprintf(2, "mystat: cannot stat %s\n", argv[i]);
      failed = 1;
      continue;
    }
    printf("%s: %s, inode %d, links %d, size %d bytes\n",
           argv[i], typename(st.type), st.ino, st.nlink, (int)st.size);
  }
  exit(failed);
}
