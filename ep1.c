#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include "List.h"
/*
Problemas ainda a resolver
Calculo do tempo real não só quando há um processo na Thread,
Como intercalar o calculo do tempo (sleep) dentro da thread e fora da thread?
*/
Fila circular;
List processos;
int tempoAtual = 0;
int isThread = 0;
int isMenor = 0;
int idExecutando = -1;
/* Rafa*/
int d = 0;
int contexto = 0;

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

	/*Implementação do Rafa*/
	thread->x.tf = tempoAtual;
	thread->x.tr = thread->x.tf - thread->x.t0;
	contexto++;
	if(d) {
		fprintf(stderr,"Encerrado Processo: %s deixando a cpu%d\n",thread->x.nome, sched_getcpu());
		fprintf(stderr,"Linha escrita no arquivo de saida: %s %d %d\n",thread->x.nome, thread->x.tf, thread->x.tr);
		fprintf(stderr, "%d mudancas de contexto \n",contexto );
	}
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
			printf("Tempo Atual: %d\n", tempoAtual);
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
	// entrada = create_list();
	ts.tv_sec  = 0;
	ts.tv_nsec = 100000000;
	if(argc < 4)
	{
		printf("Numero invalido de argumentos\n");
		exit(1);
	}
/* Rafa */
	if(argc == 5 && !strcmp(argv[4],"d")) d = 1;
	processos = lista_de_processos(argv[2]);

	pthread_t thread[processos->N];

	time_t begin;
	time(&begin);

	int i = 0;
	int qualEscalonador =	atoi(argv[1]);
	double quantum = 0.2;

	pthread_mutex_init(&mutex, NULL);
	Cell processoAtual, executando, temp;

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
				printf("Tempo Atual %d\n", tempoAtual);
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
				/*Caso não haja nenhuma thread sendo executada*/
				if (isThread == 0) {
					idExecutando = processoAtual->x.id;
					processoAtual->x.idAnterior = -1;
					executando = processoAtual;
				}
				/*Compara dt do processoAtual que chegou com o processo que está sendo executado.*/
				else if (processoAtual->x.dt < executando->x.dt) {
					processoAtual->x.idAnterior = idExecutando;
					idExecutando = processoAtual->x.id;
					executando = processoAtual;
					isMenor = 1;
				} else if (processoAtual->x.dt >= executando->x.dt) {
					if (executando->x.idAnterior == -1) {
						executando->x.idAnterior = processoAtual->x.id;
						processoAtual->x.idAnterior = -1;
					} else {
						temp = executando;
						while (temp->x.idAnterior != -1) {
							temp = at(temp->x.idAnterior,processos);
						}
						temp->x.idAnterior = processoAtual->x.id;
					}
					processoAtual->x.idAnterior = -1;				}

				if (pthread_create(&thread[i], NULL, SRTN, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&ts, NULL);
				i++;
			} else if (isThread == 0){
				sleep(1);
				printf("Tempo Atual %d\n", tempoAtual);
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

	// /*CHAMADA PARA O RR*/
	// else {
	// 	while (i < processos->N) {
	// 		processoAtual = at(i,processos);
	// 		if(processoAtual->x.t0 == tempoAtual) {
	// 			/*Ponho na fila circular*/
	// 			addProcessoFilaCircular(processoAtual->x, circular);
	// 			/*Se não tem ninguém já manda o current apontar para o atual*/
	//
	// 			/*Caso contrário, deixa quieto ele no final da fila*/
	//
	// 			/*Independente de tudo isso cria a thread*/
	// 			i++;
	// 		}	else if (isThread == 0){
	// 			sleep(1);
	// 			printf("Tempo Atual %d\n", tempoAtual);
	// 			tempoAtual++;
	// 		}
	//
	// 	}
	// }

	time_t end;
	time(&end);

	printf("Passaram-se %lf segundos\n",difftime(end,begin));
	pthread_mutex_destroy(&mutex);

/* Rafa */
	FILE *f = fopen(argv[3],"w");
	if(f == NULL)
	{
		printf("Erro ao abir\n");
		exit(1);
	}

	for(Cell p = processos->ini; p != NULL; p = p->prox)
		fprintf(f,"%s %d %d\n",p->x.nome, p->x.tf, p->x.tr);
	fprintf(f, "%d\n",contexto );

	fclose(f);

	free_list(processos);
	return 0;
}
