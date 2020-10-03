#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "List.h"
/*
Problemas ainda a resolver
Calculo do tempo real não só quando há um processo na Thread,
Como intercalar o calculo do tempo (sleep) dentro da thread e fora da thread?
*/

List processos;
int tempoAtual = 0;
struct timespec ts;

pthread_mutex_t mutex;



// Essa funcao le o arquivo s e criar uma lista ligada para guardar as informacoes dos processos

List  lista_de_processos(const char *s)
{
	int k = 0;
	FILE *p = fopen(s,"r");
	processos = create_list();
	if(p == NULL)
	{
		printf("Ocorreu erro na leitura\n");
		exit(1);
	}
	data processo;

	while(!feof(p))
	{
		fscanf(p,"%s %d %d %d",processo.nome,&processo.t0,&processo.dt,&processo.deadline);
		processo.id = k;
		addCell(processo,processos);
		k++;
	}
	fclose(p);
	return processos;
}

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
		if( j % 10 == 0) {
			count*=count;
			thread->x.dt--;
			printf("Tempo Atual: %d\n", tempoAtual);
			tempoAtual++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
	pthread_mutex_unlock(&mutex);
	printf("Encerrado Processo: %s -- Incrivel passaram-se %d segundos\n",thread->x.nome, dt);
	return NULL;
}

int main(int argc, char const *argv[])
{
	ts.tv_sec  = 0;
	ts.tv_nsec = 100000000;
	if(argc < 4)
	{
		printf("Numero invalido de argumentos\n");
		exit(1);
	}

	processos = lista_de_processos(argv[2]);

	pthread_t thread[processos->N];

	time_t begin;
	time(&begin);

	int i = 0;
	int qualEscalonador =	atoi(argv[1]);

	pthread_mutex_init(&mutex, NULL);


	/*CHAMADA PARA O FCFS*/
	if (qualEscalonador == 1) {
		for (i=0; i<processos->N; i++) {
			if (pthread_create(&thread[i], NULL, FCFS, (void*)&i)) {
					printf("\n ERROR creating thread %d\n",i);
					exit(1);
			}
			nanosleep(&ts, NULL);
		}
		for (i=0; i < processos->N; i++) {
			if (pthread_join(thread[i], NULL))  {
						printf("\n ERROR joining thread %d\n",i);
						exit(1);
			}
		}
	}

	time_t end;
	time(&end);

	printf("Passaram-se %lf segundos\n",difftime(end,begin));
	pthread_mutex_destroy(&mutex);
	free_list(processos);
	return 0;
}
