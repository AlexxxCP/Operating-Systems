CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Werror -O2
PROGRAMS = run zombie redirect pipeline pipeline3

.PHONY: all test clean
all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

test:
	bash tests/test.sh

clean:
	rm -f $(PROGRAMS)
