CC = gcc
CFLAGS = -g -Wall

all: server client

server: fileserver3.c libnetfiles.h
	$(CC) $(CFLAGS) fileserver3.c -o server -pthread

client: libnetfiles2.c client3.c libnetfiles.h
	$(CC) $(CFLAGS) libnetfiles2.c client3.c -o client

clean:
	rm client server