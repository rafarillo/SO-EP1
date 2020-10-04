CFLAGS = -g -Wall -O0 -Wno-unused-result
CC = gcc
RM = rm
#-------------------------------------------------

all: bccsh ep1

bccsh: bccsh.o
	$(CC)  bccsh.o -lreadline -o bccsh

bccsh.o: bccsh.c
	$(CC) $(CFLAGS) -c bccsh.c

ep1: ep1.o FilaCircular.o List.o Cell.o Node.o
	$(CC) ep1.o FilaCircular.o List.o Cell.o Node.o -lpthread -o ep1

ep1.o: ep1.c FilaCircular.c FilaCircular.h List.c List.h Node.c Node.h Cell.c Cell.h Data.h
	$(CC) $(CFLAGS) -c ep1.c

List.o: List.c List.h Cell.c Cell.h Data.h
	$(CC) $(CFLAGS) -c List.c

Cell.o: Cell.c Cell.h Data.h
	$(CC) $(CFLAGS) -c Cell.c

FilaCircular.o: FilaCircular.c FilaCircular.h Node.c Node.h Data.h
	$(CC) $(CFLAGS) -c FilaCircular.c

Node.o: Node.c Node.h Data.h
	$(CC) $(CFLAGS) -c Node.c

clean:
	$(RM) *.o *~
