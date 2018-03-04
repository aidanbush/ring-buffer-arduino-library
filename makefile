CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -pedantic

.PHONY: clean

test: test.c buffer.o

buffer.o: buffer.c buffer.h

clean:
	$(RM) test *.o
