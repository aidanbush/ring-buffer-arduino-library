# Author: Aidan Bush
# Date: Mar. 2, 2018
# File: makefile
# Description: makefile for ring buffer object files and tests.
SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -pedantic

.PHONY: clean

test: test.c ring_buffer.o

ring_buffer.o: ring_buffer.c ring_buffer.h

clean:
	$(RM) test *.o vgcore.*
