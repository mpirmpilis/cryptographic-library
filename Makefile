# CS-457 - Introduction to Information Security Systems
# Name: Georgios Mpirmpilis
# Makefile for Assignemnt 1 (Simple Cipher Library)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -c
OFLAGS = -Wall -Werror -Wextra -o



all: test_program.o  cs457_crypto.o
	$(CC) $(OFLAGS) cipher test_program.o cs457_crypto.o



test_program.o: test_program.c 
	$(CC) $(CFLAGS) test_program.c


cs457_crypto.o: cs457_crypto.c
	$(CC) $(CFLAGS) cs457_crypto.c




.PHONY: clean
clean:
	rm -rf *.o cipher