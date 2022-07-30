CC = cc
# These library locations probably won't work on an ARM Mac so probably figure this out
CFLAGS = -Wall -Wextra -std=c99 -I/usr/local/include -L/usr/local/lib -lglfw -lvulkan

all: lab

lab: main.c
	$(CC) main.c $(CFLAGS) -o lab

run: lab
	./lab

clean:
	rm -f *.o lab
