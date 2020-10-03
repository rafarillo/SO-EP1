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

List processos;
int tempoAtual = 0;
int validador = 0;
struct timespec ts;
int d = 0;
pthread_mutex_t mutex;

int contexto = 0;

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

// void * escalondor(void * i) {
// 	trava o FCFS;
// 	e destrava[]
// }


void * FCFS(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	int dt = thread->x.dt;

	// while(identificadorGlobal != thread->x.id) {
	// 	sleep(1);
	// }

	pthread_mutex_lock(&mutex);
	if(d) fprintf(stderr,"Sou o processo %s usando a cpu%d\n",thread->x.nome,sched_getcpu());

	/*Seção Crítica*/
	while(thread->x.dt > 0)
	{
		// if (chegou menor) {
		//
		// 	chegou menor = 0;
		// 	pthread_mutex_unlock(&mutex);
		// 	while(identificadorGlobal != thread->x.id) {
		// 		sleep(1);
		// 	}
		// }
		if( j % 10 == 0) {
			count*=count;
			thread->x.dt--;
			printf("Tempo Atual: %d\n", tempoAtual);
			tempoAtual++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
	// de o pop se a pilha não estiver vazia
	// identificadorGlobal = thread->x.idanterior;
	pthread_mutex_unlock(&mutex);
	thread->x.tf = tempoAtual;
	thread->x.tr = thread->x.tf - thread->x.t0;
	contexto++;
	if(d) {
		fprintf(stderr,"Encerrado Processo: %s deixando a cpu%d\n",thread->x.nome, sched_getcpu());
		fprintf(stderr,"Linha escrita no arquivo de saida: %s %d %d\n",thread->x.nome, thread->x.tf, thread->x.tr);
	}
	fprintf(stderr, "%d mudancas de contexto \n",contexto );
	return NULL;
}

/*
void * SRTN(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	int dt = thread->x.dt;

	pthread_mutex_lock(&mutex);
	printf("Sou o processo %s\n",thread->x.nome);
	Seção Crítica
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
*/


int main(int argc, char const *argv[])
{
	ts.tv_sec  = 0;
	ts.tv_nsec = 100000000;
	if(argc < 4)
	{
		printf("Numero invalido de argumentos\n");
		exit(1);
	}
	if(argc == 5 && !strcmp(argv[4],"d")) d = 1;
	processos = lista_de_processos(argv[2]);
	pthread_t thread[processos->N];

	time_t begin;
	time(&begin);

	int i = 0;
	int qualEscalonador =	atoi(argv[1]);

	pthread_mutex_init(&mutex, NULL);
	Cell processoAtual;
	int vdd = 0;
	/*CHAMADA PARA O FCFS*/
	if (qualEscalonador == 1) {
		while (i < processos->N) {
			processoAtual = at(i,processos);
			if(processoAtual->x.t0 == tempoAtual) {
				if(d)  fprintf(stderr,"Processo %s %d %d %d pede acesso -- no tempo: %d\n", processoAtual->x.nome,processoAtual->x.t0,processoAtual->x.dt,processoAtual->x.deadline ,tempoAtual);
				vdd = 1;
				if (pthread_create(&thread[i], NULL, FCFS, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&ts, NULL);
				i++;
			} else if (vdd == 0){
				sleep(1);
				printf("tempoAtual %d\n", tempoAtual);
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
				/*Comparar dt do processo que chegou com o processo que está sendo executado.*/
				/*Caso dt seja menor, a gente cria a nova thread e retira o outro processo*/

				/*
				if (dt < dt)
					processoAtual->x.idanterior = identificadorGlobal;
					identificadorGlobal = processoAtual->id
					chegou menor*/

				/*Caso contrário, colocamos em uma lista*/
				/*
				if (pthread_create(&thread[i], NULL, SRTN, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&ts, NULL);
				i++;
				*/
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
