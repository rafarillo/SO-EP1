#include "Node.h"
#include <stdio.h>
#define bool int

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
void addProcessoFilaCircular(data, Fila, int);

/*Imprime a fila em determinado UT*/
// void imprimeFila(Fila, int);

/*Retorna o tamanho da Fila*/
int filaSize(Fila);

/*Devolve true se a fila está vazia*/
bool filaIsEmpty(Fila);

/*Retira processo da fila da CPU*/
Link retiraNodeFilaCircular(Fila);

/*CURRENT aponta para o próximo Node*/
void proximoNode(Fila);

/*Destrutor*/
void filaFree();
