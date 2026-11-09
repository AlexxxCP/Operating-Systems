CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Werror -O2

.PHONY: all test clean
all: syscalls

syscalls: syscalls.c
	$(CC) $(CFLAGS) $< -o $@

test:
	bash tests/test.sh

clean:
	rm -f syscalls
