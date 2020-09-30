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
// int inicioSemProcesso = 1;

// Essa funcao le o arquivo s e criar uma lista ligada para guardar as informacoes dos processos

List  lista_de_processos(const char *s)
{
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
		addCell(processo,processos);
	}
	fclose(p);
	return processos;
}

void * FCFS(void * i)
{
	int * P_i = (int *) i;
	long int count = 2;
	// inicioSemProcesso = 0;
	Cell thread = at(*P_i,processos);
	printf("Sou o processo %s\n",thread->x.nome);
	int dt = thread->x.dt;
	while(thread->x.dt > 0)
	{
		count*=count;
		sleep(1);
		thread->x.dt--;
		tempoAtual++;
		printf("Tempo Atual: %d\n", tempoAtual);
	}
	printf("Incrivel passaram-se %d segundos\n",dt);
	return NULL;
}

int main(int argc, char const *argv[])
{
	if(argc < 4)
	{
		printf("Numero invalido de argumentos\n");
		exit(1);
	}

	processos = lista_de_processos(argv[2]);

	pthread_t thread;

	time_t begin;
	time(&begin);

	int i = 0;

	while (i < processos->N) {

		if(pthread_create(&thread, NULL, FCFS, (void *) &i))
		{
			printf("Erro ao criar a thread \n");
			exit(1);
		}
		if(pthread_join(thread,NULL))
		{
			printf("Erro ao dar join \n");
			exit(1);
		}
		// if (inicioSemProcesso)
		// {
		// 	sleep(1);
		// 	tempoAtual++;
		// }
		i++;
	}

	time_t end;
	time(&end);

	printf("Passaram-se %lf segundos\n",difftime(end,begin));
	free_list(processos);
	return 0;
}
