CFLAGS = -g -Wall -O0 -Wno-unused-result 
CC = gcc
RM = rm 
#-------------------------------------------------

bccsh: bccsh.o
	$(CC)  bccsh.o -lreadline -o bccsh

bccsh.o: bccsh.c
	$(CC) $(CFLAGS) -c bccsh.c

clean:
	$(RM) *.o *~