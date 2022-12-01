CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon
DBFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon -g -fsanitize=address

SRC_DIR := src
INC_DIR := include

EXE := main
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:.c=.o)

.PHONY: all debug doc clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
#debug
debug: $(OBJ)
	$(CC) $(DBFLAGS) -o $(EXE) $^ -lm -g
# latex documentation generate
doc:
	cd doc && xelatex dokumentace.tex && rm -f *.aux *.log *.out *.toc *.fls *.fdb_latexmk
clean:
	rm -f $(OBJ) $(EXE)
