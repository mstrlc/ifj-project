CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
#debug flags
DBFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g -fsanitize=address

#Use this to compile the program
lexer: lexer.o symtable.o
	$(CC) $(CFLAGS) -o lexer lexer.o symtable.o
#Use this for debugging
debug: lexer.o symtable.o
	$(CC) $(DBFLAGS) -o lexer lexer.o symtable.o
lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c

clean: 
	@rm -f lexer *.o