/* Reconstructed from Lab 8, with setup and byte-transfer error checks added. */
#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int p2c[2], c2p[2];
  char b = 'x';
  if(pipe(p2c) < 0){
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }
  if(pipe(c2p) < 0){
    close(p2c[0]); close(p2c[1]);
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }
  int pid = fork();
  if(pid < 0){
    close(p2c[0]); close(p2c[1]); close(c2p[0]); close(c2p[1]);
    fprintf(2, "pingpong: fork failed\n");
    exit(1);
  }
  if(pid == 0){
    close(p2c[1]); close(c2p[0]);
    if(read(p2c[0], &b, 1) != 1){
      fprintf(2, "pingpong: read failed\n");
      exit(1);
    }
    printf("%d: received ping\n", getpid());
    if(write(c2p[1], &b, 1) != 1){
      fprintf(2, "pingpong: write failed\n");
      exit(1);
    }
    close(p2c[0]); close(c2p[1]);
    exit(0);
  }
  close(p2c[0]); close(c2p[1]);
  if(write(p2c[1], &b, 1) != 1){
    close(p2c[1]); close(c2p[0]);
    fprintf(2, "pingpong: write failed\n");
    wait(0);
    exit(1);
  }
  close(p2c[1]);
  int received = read(c2p[0], &b, 1);
  close(c2p[0]);
  if(received == 1)
    printf("%d: received pong\n", getpid());
  else
    fprintf(2, "pingpong: read failed\n");
  int status;
  if(wait(&status) < 0 || received != 1)
    exit(1);
  exit(status);
}
