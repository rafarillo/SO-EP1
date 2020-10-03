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
List entrada;
int tempoAtual = 0;
int isThread = 0;
int isMenor = 0;
int idExecutando = -1;
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
		fscanf(p,"%s%d%d%d\n",processo.nome,&processo.t0,&processo.dt,&processo.deadline);
		processo.id = k;
		processo.idAnterior = -2;
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
	isThread++;
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
	isThread--;
	printf("Encerrado Processo: %s -- Incrivel passaram-se %d segundos\n",thread->x.nome, dt);
	return NULL;
}

void * SRTN(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	int dt = thread->x.dt;
	isThread++;
	while(idExecutando != thread->x.id) {
		nanosleep(&ts, NULL);
	}
	pthread_mutex_lock(&mutex);
	isMenor = 0;

	printf("Sou o processo %s\n",thread->x.nome);

	/*Seção Crítica*/
	while(thread->x.dt > 0)
	{
		if (isMenor) {
			isMenor = 0;
			pthread_mutex_unlock(&mutex);
			while(idExecutando != thread->x.id) {
				nanosleep(&ts, NULL);
			}
		}
		if( j % 10 == 0) {
			count*=count;
			thread->x.dt--;
			printf("Tempo Atual: %d -- Processo: %s\n", tempoAtual, thread->x.nome);
			tempoAtual++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
	pthread_mutex_unlock(&mutex);
	idExecutando = thread->x.idAnterior;
	isThread--;
	printf("Encerrado Processo: %s -- Incrivel passaram-se %d segundos\n",thread->x.nome, dt);
	return NULL;
}


int main(int argc, char const *argv[])
{
	entrada = create_list();
	ts.tv_sec  = 0;
	ts.tv_nsec = 100000000;
	if(argc < 4)
	{
		printf("Numero invalido de argumentos\n");
		exit(1);
	}

	processos = lista_de_processos(argv[2]);
	printf("Numero de Processos: %d\n", processos->N);
	for (int l = 0; l < processos->N; l++) {
		printf("Nome %s\n", at(l,processos)->x.nome);
	}
	pthread_t thread[processos->N];

	time_t begin;
	time(&begin);

	int i = 0;
	int qualEscalonador =	atoi(argv[1]);

	pthread_mutex_init(&mutex, NULL);
	Cell processoAtual;
	Cell executando;
	Cell temp, temp1;

	/*CHAMADA PARA O FCFS*/
	if (qualEscalonador == 1) {
		while (i < processos->N) {
			processoAtual = at(i,processos);
			if(processoAtual->x.t0 == tempoAtual) {
				printf("Processo %s pede acesso -- no tempo: %d\n", processoAtual->x.nome, tempoAtual);
				if (pthread_create(&thread[i], NULL, FCFS, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&ts, NULL);
				i++;
			} else if (isThread == 0){
				sleep(1);
				printf("--tempoAtual %d\n", tempoAtual);
				tempoAtual++;
			}
		}
		for (i=0; i < processos->N; i++) {
			if (pthread_join(thread[i], NULL))  {
						printf("\n ERROR joining thread %d\n",i);
						exit(1);
			}
		}
		printf("\n\n%d\n", tempoAtual);
	}
	/* CHAMADA PARA O SRTN */
	else if (qualEscalonador == 2) {

		while (i < processos->N) {
			processoAtual = at(i,processos);
			if(processoAtual->x.t0 == tempoAtual) {
				printf("Processo %s pede acesso -- no tempo: %d\n", processoAtual->x.nome, tempoAtual);
				addCell()
				/*Caso não haja nenhuma thread sendo executada*/
				if (isThread == 0) {
					printf("ZERO\n");
					idExecutando = processoAtual->x.id;
					processoAtual->x.idAnterior = -1;
					executando = processoAtual;
					printf("Nome %s -- Anterior p%d\n", processoAtual->x.nome, processoAtual->x.idAnterior);
				}
				/*Compara dt do processoAtual que chegou com o processo que está sendo executado.*/
				else if (processoAtual->x.dt < executando->x.dt) {
					printf("MENOR\n");
					printf("Processo Atual: %s\n", processoAtual->x.nome);
					printf("Executando: %s\n", executando->x.nome);
					processoAtual->x.idAnterior = idExecutando;
					idExecutando = processoAtual->x.id;
					executando = processoAtual;
					isMenor = 1;
					printf("Nome %s -- Anterior p%d\n", processoAtual->x.nome, processoAtual->x.idAnterior);
				} else if (processoAtual->x.dt >= executando->x.dt) {
					printf("MAIOR\n");
					if (executando->x.idAnterior == -1) {
						executando->x.idAnterior = processoAtual, temp->x.idAnterior->x.id;
					} else {
						temp = executando;
						temp1 = temp;
						while (temp->x.idAnterior != -1) {
							temp1 = temp;
							temp = at(temp->x.idAnterior,processos);
						}
						printf("-----%s\n", temp1->x.nome);
						temp1->x.idAnterior = processoAtual->x.id;
					}
					processoAtual->x.idAnterior = -1;
					printf("Nome %s -- Anterior p%d\n", processoAtual->x.nome, processoAtual->x.idAnterior);
				}
				for (int l = 0; l < processos->N; l++) {
					temp = at(l,processos);
					printf("Nome %s -- Anterior p%d\n", temp->x.nome, temp->x.idAnterior);
				}
				if (pthread_create(&thread[i], NULL, SRTN, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&ts, NULL);
				i++;
			} else if (isThread == 0){
				sleep(1);
				printf("--tempoAtual %d\n", tempoAtual);
				tempoAtual++;
			}
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
