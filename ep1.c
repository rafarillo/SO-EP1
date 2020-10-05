#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include "List.h"

Fila circular;
List processos;
int tempoAtual = 0;
int isThread = 0;
int isMenor = 0;
int idExecutando = -1;
double quantum = 3;
int d = 0;
int contexto = 0;

struct timespec ts;
struct timespec tsrr;
pthread_mutex_t mutex;



/*Essa funcao le o arquivo 's' e cria uma lista ligada para guardar as informacoes dos processos*/
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

/*Funcao do escalonador FCFS onde enviamos as threads*/
void * FCFS(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	isThread++;
	pthread_mutex_lock(&mutex);
	if(d) fprintf(stderr,"Sou o processo %s usando a cpu%d\n",thread->x.nome,sched_getcpu());

	/*Seção Crítica*/
	while(thread->x.dt > 0)
	{
		if( j % 10 == 0) {
			count*=count;
			thread->x.dt--;
			tempoAtual++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
	pthread_mutex_unlock(&mutex);
	thread->x.tf = tempoAtual;
	thread->x.tr = thread->x.tf - thread->x.t0;
	contexto++;
	if(d) {
		fprintf(stderr,"Encerrado Processo: %s deixando a cpu%d\n",thread->x.nome, sched_getcpu());
		fprintf(stderr,"Linha escrita no arquivo de saida: %s %d %d\n",thread->x.nome, thread->x.tf, thread->x.tr);
		fprintf(stderr, "%d mudancas de contexto \n",contexto );
	}
	isThread--;
	return NULL;
}

/*Funcao do escalonador SRTN onde enviamos as threads*/
void * SRTN(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	isThread++;
	while(idExecutando != thread->x.id) {
		nanosleep(&ts, NULL);
	}
	pthread_mutex_lock(&mutex);
	if(d) fprintf(stderr,"Sou o processo %s usando a cpu%d\n",thread->x.nome,sched_getcpu());
	isMenor = 0;

	/*Seção Crítica*/
	while(thread->x.dt > 0)
	{
		if (isMenor) {
			isMenor = 0;
			pthread_mutex_unlock(&mutex);
			contexto++;
			while(idExecutando != thread->x.id) {
				nanosleep(&ts, NULL);
			}
		}
		if( j % 10 == 0) {
			count*=count;
			thread->x.dt--;
			tempoAtual++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
	pthread_mutex_unlock(&mutex);
	idExecutando = thread->x.idAnterior;

	thread->x.tf = tempoAtual;
	thread->x.tr = thread->x.tf - thread->x.t0;
	contexto++;
	if(d) {
		fprintf(stderr,"Encerrado Processo: %s deixando a cpu%d\n",thread->x.nome, sched_getcpu());
		fprintf(stderr,"Linha escrita no arquivo de saida: %s %d %d\n",thread->x.nome, thread->x.tf, thread->x.tr);
		fprintf(stderr, "%d mudancas de contexto \n",contexto );
	}
	isThread--;
	return NULL;
}
/*Funcao do escalonador ROUND ROBIN onde enviamos as threads*/
void * RR(void * i)
{
	int j = 1;
	int * P_i = (int *) i;
	long int count = 2;
	Cell thread = at(*P_i,processos);
	isThread++;

	while(idExecutando != thread->x.id) {
		nanosleep(&ts, NULL);
	}
	pthread_mutex_lock(&mutex);
	if(d) fprintf(stderr,"Sou o processo %s usando a cpu%d\n",thread->x.nome,sched_getcpu());

	/*Seção Crítica*/
	while(thread->x.dt > 0)
	{
		if (thread->x.tquantum == quantum) {
			contexto++;
			thread->x.tquantum = 0;
			proximoNode(circular);
			idExecutando = circular->current->celula->x.id;
			pthread_mutex_unlock(&mutex);
			while(idExecutando != thread->x.id) {
				nanosleep(&ts, NULL);
			}
		}
		if( j % 10 == 0) {
			count*=count;
			thread->x.dt--;
			tempoAtual++;
			thread->x.tquantum++;
		}
		nanosleep(&ts, NULL);
		j++;
	}
	pthread_mutex_unlock(&mutex);
	isThread--;
	thread->x.tf = tempoAtual;
	thread->x.tr = thread->x.tf - thread->x.t0;
	contexto++;
	if(d) {
		fprintf(stderr,"Encerrado Processo: %s deixando a cpu%d\n",thread->x.nome, sched_getcpu());
		fprintf(stderr,"Linha escrita no arquivo de saida: %s %d %d\n",thread->x.nome, thread->x.tf, thread->x.tr);
		fprintf(stderr, "%d mudancas de contexto \n",contexto );
	}
	retiraNodeFilaCircular(circular);
	idExecutando = circular->current->celula->x.id;
	return NULL;
}

int main(int argc, char const *argv[])
{
	circular = filaInit();
	ts.tv_sec  = 0;
	ts.tv_nsec = 100000000; // 1/10 s

	tsrr.tv_sec  = 0;
	tsrr.tv_nsec = 100; // 1/10000000 s
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

	pthread_mutex_init(&mutex, NULL);
	Cell processoAtual, executando, temp;

	/*CHAMADA PARA O FCFS*/
	if (qualEscalonador == 1) {
		while (i < processos->N) {
			/*Pega o i-ésimo processo*/
			processoAtual = at(i,processos);
			/*Confere se o t0 do processoAtual é equivalente ao tempoAtual*/
			if(processoAtual->x.t0 == tempoAtual) {
				if(d)  fprintf(stderr,"Processo %s %d %d %d pede acesso -- no tempo: %d\n", processoAtual->x.nome,processoAtual->x.t0,processoAtual->x.dt,processoAtual->x.deadline ,tempoAtual);
				if (pthread_create(&thread[i], NULL, FCFS, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&tsrr, NULL);
				i++;
			}
			/*Caso não haja algum processo sendo executado na thread, conte o tempo*/
			else if (isThread == 0){
				sleep(1);
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
	/* CHAMADA PARA O SRTN */
	else if (qualEscalonador == 2) {

		while (i < processos->N) {
			/*Pega o i-ésimo processo*/
			processoAtual = at(i,processos);
			/*Confere se o t0 do processoAtual é equivalente ao tempoAtual*/
			if(processoAtual->x.t0 == tempoAtual) {
				if(d)  fprintf(stderr,"Processo %s %d %d %d pede acesso -- no tempo: %d\n", processoAtual->x.nome,processoAtual->x.t0,processoAtual->x.dt,processoAtual->x.deadline ,tempoAtual);
				/*Caso não haja nenhuma thread sendo executada, execute o processo atual*/
				if (isThread == 0) {
					idExecutando = processoAtual->x.id;
					processoAtual->x.idAnterior = -1;
					executando = processoAtual;
				}
				/*Compara dt do processoAtual que chegou, com o processo que está sendo executado.*/
				/*Caso dt de processo atual seja menor, execute o processo, e mande o que estava sendo executado, para segundo da fila*/
				else if (processoAtual->x.dt < executando->x.dt) {
					processoAtual->x.idAnterior = idExecutando;
					idExecutando = processoAtual->x.id;
					executando = processoAtual;
					isMenor = 1;
				}
				/*Caso dt de processo atual seja maior, mande o processo atual para o fim da fila*/
				else if (processoAtual->x.dt >= executando->x.dt) {
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
					processoAtual->x.idAnterior = -1;
				}
				if (pthread_create(&thread[i], NULL, SRTN, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&tsrr, NULL);
				i++;
			}
			/*Caso não haja algum processo sendo executado na thread, conte o tempo*/
			else if (isThread == 0){
				sleep(1);
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

	/*CHAMADA PARA O RR*/
	else {
		while (i < processos->N) {
			/*Pega o i-ésimo processo*/
			processoAtual = at(i,processos);

			/*Confere se o t0 do processoAtual é equivalente ao tempoAtual*/
			if(processoAtual->x.t0 == tempoAtual) {
				if(d)  fprintf(stderr,"Processo %s %d %d %d pede acesso -- no tempo: %d\n", processoAtual->x.nome,processoAtual->x.t0,processoAtual->x.dt,processoAtual->x.deadline ,tempoAtual);

				addProcessoFilaCircular(processoAtual, circular);

				/*Se não há processo sendo executado na thread, mande o próximo processo*/
				if (isThread == 0) {
					proximoNode(circular);
					idExecutando = circular->current->celula->x.id;
				}
				if (pthread_create(&thread[i], NULL, RR, (void*)&i)) {
						printf("\n ERROR creating thread %d\n",i);
						exit(1);
				}
				nanosleep(&tsrr, NULL);
				i++;
			}
			/*Caso não haja algum processo sendo executado na thread, conte o tempo*/
			else if (isThread == 0){
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
	}

	time_t end;
	time(&end);
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
	filaFree(circular);
	return 0;
}
