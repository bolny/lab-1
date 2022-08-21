CC = clang
CFLAGS =   -std=c99 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS +=  -Ilib/glad/include -Ilib/glfw/include
LDFLAGS =  lib/glad/src/gl.o lib/glfw/src/libglfw3.a
LDFLAGS += -framework OpenGL -framework Cocoa -framework IOKit
LDFLAGS += -framework CoreVideo

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY = all clean run

DEFAULT = all

all: dirs libs lab

libs: glad glfw

glad:
	cd lib/glad && $(CC) -o src/gl.o -Iinclude -c src/gl.c

glfw:
	cd lib/glfw && cmake . && make

dirs:
	mkdir -p ./$(BIN)

lab: $(OBJ)
	$(CC) -o $(BIN)/lab $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

run: all
	$(BIN)/lab
