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

List create_list();
void addCell(data x,List l);
Cell  at(int i, List l);
void dump_list();
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

/*Imprime a fila em determinado UT*/
// void imprimeFila(Fila, int);

/*Retorna o tamanho da Fila*/
int filaSize(Fila);

/*Devolve true se a fila está vazia*/
bool filaIsEmpty(Fila);

/*Retira processo da fila da CPU*/
void retiraNodeFilaCircular(Fila);

/*CURRENT aponta para o próximo Node*/
void proximoNode(Fila);

/*Destrutor*/
void filaFree();
