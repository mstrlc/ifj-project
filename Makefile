CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
#debug flags
DBFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g -fsanitize=address

#Use this to compile the program
all: main.o lexer.o symtable.o parser.o
	$(CC) $(CFLAGS) -o main main.o lexer.o symtable.o parser.o
	
lexer: lexer.o symtable.o
	$(CC) $(CFLAGS) -o lexer lexer.o symtable.o
#Use this for debugging
debug: lexer.o symtable.o
	$(CC) $(DBFLAGS) -o lexer lexer.o symtable.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c

clean:
	@rm -f *.o main lexer
	@rm -f  *.o
	@rm -f a.out