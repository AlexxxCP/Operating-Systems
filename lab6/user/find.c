/*
 * Reconstructed from the directory loop in Lab 6.
 * Surrounding setup, bounds checks and error handling complete the excerpt.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static int
find(char *path, char *target)
{
  char buf[512], name[DIRSIZ + 1], *p;
  struct dirent de;
  struct stat st;
  int fd, n, failed = 0;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return 1;
  }
  if(fstat(fd, &st) < 0 || st.type != T_DIR){
    fprintf(2, "find: not a readable directory: %s\n", path);
    close(fd);
    return 1;
  }
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "find: path too long: %s\n", path);
    close(fd);
    return 1;
  }
  strcpy(buf, path);
  p = buf + strlen(buf);
  if(p == buf || p[-1] != '/')
    *p++ = '/';

  while((n = read(fd, &de, sizeof(de))) == (int)sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(name, de.name, DIRSIZ);
    name[DIRSIZ] = 0;
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;
    strcpy(p, name);
    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", buf);
      failed = 1;
      continue;
    }
    if(strcmp(name, target) == 0)
      printf("%s\n", buf);
    if(st.type == T_DIR && find(buf, target) != 0)
      failed = 1;
  }
  if(n < 0){
    fprintf(2, "find: read error: %s\n", path);
    failed = 1;
  }
  close(fd);
  return failed;
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "usage: find <dir> <name>\n");
    exit(1);
  }
  exit(find(argv[1], argv[2]));
}
