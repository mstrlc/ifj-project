CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g

lexer: lexer.o symtable.o
	$(CC) $(CFLAGS) -o lexer lexer.o symtable.o
lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c


symtable.o: symtable.c symtable.h
clean: 
	@rm -f lexer *.o