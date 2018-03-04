# Author: Aidan Bush
# Date: Mar. 2, 2018
# File: makefile
# Description: makefile for ring buffer object files and tests.
CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -pedantic

.PHONY: clean

test: test.c buffer.o

buffer.o: buffer.c buffer.h

clean:
	$(RM) test *.o
