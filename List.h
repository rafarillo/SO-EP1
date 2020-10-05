#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cell.h"
#define bool int

/*Definição da Lista dos processos*/
typedef struct list *List;
struct list
{
	Cell ini;
	int N;
};
/*Cria lista dos processos*/
List create_list();

/*Adiciona uma célula à lista*/
void addCell(data x,List l);

/*Retira o i-esimo elemento da lista*/
Cell  at(int i, List l);

/*Imprime lista*/
void dump_list();

/*Destrutor*/
void free_list(List l);

int number_elements();

/*Definição para Fila circular dos processos*/
typedef struct sequenciaFila* Fila;
struct sequenciaFila {
  Link cabeca;
  Link last;
  Link current;
  /*Armazena o tamanho*/
  int n;
};

/*Construtor*/
Fila filaInit();

/*Adiciona Processo na Fila da CPU*/
void addProcessoFilaCircular(Cell, Fila);

/*Retira processo da fila da CPU*/
void retiraNodeFilaCircular(Fila);

/*CURRENT aponta para o próximo Node*/
void proximoNode(Fila);

/*Destrutor*/
void filaFree();
