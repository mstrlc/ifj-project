CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon

lexer: lexer.o symtable.o
	$(CC) $(CFLAGS) -o lexer lexer.o symtable.o
lexer.o: lexer.c lexer.h
#	$(CC) -c lexer.c


symtable.o: symtable.c symtable.h
clean: 
	@rm -f lexer *.o