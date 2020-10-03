#include "FCFS.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "List.h"


void * FCFS(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	int dt = thread->x.dt;

	pthread_mutex_lock(&mutex);
	printf("Sou o processo %s\n",thread->x.nome);
	/*Seção Crítica*/
	while(thread->x.dt > 0)
	{
		if(j%10 == 0) {
			count*=count;
			thread->x.dt--;
			printf("Tempo Atual: %d\n", tempoAtual);
			tempoAtual++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
