CC = clang
CFLAGS =  -std=c99 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Ilib/glad/include
LDFLAGS = lib/glad/src/glad.o

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY = all clean

DEFAULT = all

all: dirs libs lab

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c

dirs:
	mkdir -p ./$(BIN)

lab: $(OBJ)
	$(CC) -o $(BIN)/lab $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
