#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "process.h"

List *processos;

// Essa funcao le o arquivo s e criar uma lista ligada para guardar as informacoes dos processos

List * lista_de_processos(const char *s)
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
		create_cell(processo,processos);
	}	
	fclose(p);
	return processos;
}

void * FCFS(void * i)
{
	int * P_i = (int *) i;
	long int count = 2;
	struct cell *thread = at(*P_i,processos);
	printf("Sou o processo %s\n",thread->x.nome);
	int dt = thread->x.dt;
	while(thread->x.dt > 0)
	{
		count*=count;
		sleep(1);
		thread->x.dt--;
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
	
	List *processos = lista_de_processos(argv[2]);
	//dump_list(processos);
	//printf("N=%d\n",processos->N);
	pthread_t threads[processos->N];

	time_t begin;
	time(&begin);
	for(int i = 0; i < processos->N; i++)
	{
		if(pthread_create(&threads[i], NULL, FCFS, (void *) &i))
		{
			printf("Erro ao criar a thread \n");
			exit(1);
		}
		if(pthread_join(threads[i],NULL))
		{
			printf("Erro ao dar join \n");
			exit(1);
		}

	}
	
	time_t end;
	time(&end);

	printf("Passaram-se %lf segundos\n",difftime(end,begin));
	free_list(processos);
	return 0;
}