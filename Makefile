CFLAGS = -g -Wall -O0 -Wno-unused-result 
CC = gcc
RM = rm 
#-------------------------------------------------

all: bccsh ep1

bccsh: bccsh.o
	$(CC)  bccsh.o -lreadline -o bccsh

bccsh.o: bccsh.c
	$(CC) $(CFLAGS) -c bccsh.c

ep1: ep1.o process.h
	$(CC) ep1.o process.o -lpthread -o ep1


ep1.o: ep1.c process.o process.h
	$(CC) $(CFLAGS) -c ep1.c

process.o: process.c process.h
	$(CC) $(CFLAGS) -c process.c

clean:
	$(RM) *.o *~