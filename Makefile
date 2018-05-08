CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = htable
OBJS = htable.o ht_funcs.o
all: $(MAIN)

$(MAIN): $(OBJS) htable.c ht_funcs.c
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

htable.o: htable.c
	$(CC) $(CFLAGS) -c htable.c
ht_funcs.o: ht_funcs.c
	$(CC) $(CFLAGS) -c ht_funcs.c

clean:
	rm *.o $(MAIN) core
