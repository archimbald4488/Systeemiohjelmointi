CC=gcc
CFLAGS=-Wall -Werror -pthread -g

all: pzip punzip punzip2

pzip: pzip.c
	$(CC) $(CFLAGS) pzip.c -o pzip

punzip: punzip.c
	$(CC) $(CFLAGS) punzip.c -o punzip

punzip2: punzip2.c
	$(CC) -Wall -Werror -g -o punzip2

clean:
	rm -f pzip punzip punzip2
