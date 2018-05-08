CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = htable hencode hdecode
OBJS = hencode.o hdecode.o htable.o

all: $(MAIN)

hencode: hencode.o
	$(CC) $(CFLAGS) -o hencode hencode.o
hencode.o: hencode.c
	$(CC) $(CFLAGS) -c hencode.c ht_funcs.h

hdecode: hdecode.o
	$(CC) $(CFLAGS) -o hdecode hdecode.o
hdecode.o: hdecode.c
	$(CC) $(CFLAGS) -c hdecode.c ht_funcs.h

htable: htable.o
	$(CC) $(CFLAGS) -o htable htable.o
htable.o: htable.c
	$(CC) $(CFLAGS) -c htable.c ht_funcs.h

clean:
	rm *.o $(MAIN) core


