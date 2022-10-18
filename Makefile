CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon

lexer: lexer.o 
	$(CC) $(CFLAGS) -o lexer lexer.o
lexer.o: lexer.c lexer.h
#	$(CC) -c lexer.c
clean: 
	@rm -f lexer *.o