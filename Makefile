CFLAGS = -g -Wall -O0 -Wno-unused-result
CC = gcc
RM = rm
#-------------------------------------------------

all: bccsh ep1

bccsh: bccsh.o
	$(CC)  bccsh.o -lreadline -o bccsh

bccsh.o: bccsh.c
	$(CC) $(CFLAGS) -c bccsh.c

ep1: ep1.o List.o Cell.o
	$(CC) ep1.o List.o Cell.o -lpthread -o ep1

ep1.o: ep1.c List.c List.h Cell.c Cell.h Data.h
	$(CC) $(CFLAGS) -c ep1.c

List.o: List.c List.h Cell.c Cell.h Data.h
	$(CC) $(CFLAGS) -c List.c

Cell.o: Cell.c Cell.h Data.h
	$(CC) $(CFLAGS) -c Cell.c

clean:
	$(RM) *.o *~
