CC = cc
CFLAGS = -Wall -Wextra -std=c99 \
				 -I/opt/homebrew/include \
				 -L/opt/homebrew/lib \
				 -lglfw \
				 -lMoltenVK \
				 -lvulkan \

all: lab

lab: src/main.c
	$(CC) src/main.c $(CFLAGS) -o bin/lab

clean:
	rm -f tmp/* bin/*
