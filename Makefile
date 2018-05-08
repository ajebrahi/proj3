CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = htable hencode hdecode
OBJS = hencode.o hdecode.o htable.o

all: $(MAIN)

hencode: hencode.o
	$(CC) $(CFLAGS) -o hencode hencode.o
hencode.o: hencode.c
	$(CC) $(CFLAGS) -c hencode.c

hdecode: hdecode.o
	$(CC) $(CFLAGS) -o hdecode hdecode.o
hdecode.o: hdecode.c
	$(CC) $(CFLAGS) -c hdecode.c

htable: htable.o
	$(CC) $(CFLAGS) -o htable htable.o
htable.o: htable.c
	$(CC) $(CFLAGS) -c htable.c

clean:
	rm *.o $(MAIN) core


