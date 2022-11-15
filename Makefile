CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
#debug flags
DBFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g -fsanitize=address

SRC_DIR := src
INC_DIR := include

EXE := main
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:.c=.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
#debug
debug: $(OBJ)
	$(CC) $(DBFLAGS) -o $@ $^
clean:
	rm -f $(OBJ) $(EXE)
