CFLAGS = -g -Wall -O0 -Wno-unused-result 
CC = gcc
RM = rm 
#-------------------------------------------------

bccshell: EP1.o
	$(CC)  EP1.o -lreadline -o bccshell

EP1.o: EP1.c
	$(CC) $(CFLAGS) -c EP1.c

clean:
	$(RM) *.o *~