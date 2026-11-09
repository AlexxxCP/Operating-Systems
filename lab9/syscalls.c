/* Transcribed from Lab 9; checks for read/write/open/close failures were added. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
main(void)
{
    const char *msg = "hello, kernel\n";
    size_t length = strlen(msg), sent = 0;
    int fd = open("note.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }
    printf("open returned descriptor %d\n", fd);
    while (sent < length) {
        ssize_t n = write(fd, msg + sent, length - sent);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0) { perror("write"); close(fd); return 1; }
        sent += (size_t)n;
    }
    printf("write sent %zu bytes\n", sent);
    if (close(fd) < 0) { perror("close"); return 1; }
    fd = open("note.txt", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    char buf[64];
    ssize_t n;
    do { n = read(fd, buf, sizeof(buf) - 1); } while (n < 0 && errno == EINTR);
    if (n < 0) { perror("read"); close(fd); return 1; }
    buf[n] = '\0';
    printf("read got %zd bytes: %s", n, buf);
    if (close(fd) < 0) { perror("close"); return 1; }
    return 0;
}
