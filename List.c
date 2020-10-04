#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#define bool int


/* Protótipo de rotinas auxiliares */
static void *mallocSafe(size_t nbytes);

/*Rotinas para Lista dos processos*/
List create_list()
{
	List l = mallocSafe(sizeof(List));
	l->ini = NULL;
	l->N = 0;
	return l;
}

void addCell(data x,List l)
{
	Cell new_node = create_cell(x, NULL);
	if(l->ini == NULL) l->ini = new_node;
	else
	{
		Cell p;
		for(p = l->ini; p->prox != NULL; p = p->prox);
		p->prox = new_node;
	}
	l->N++;
}

Cell at(int i, List l)
{
	Cell p;
	int k;
	for(k = 0, p = l->ini; k < i; p=p->prox,k++);
	return p;
}

void dump_list(List l)
{
	for(Cell p = l->ini; p != NULL; p = p->prox)
		printf("%s %d %d %d\n",p->x.nome,p->x.t0,p->x.dt,p->x.deadline);
}

void free_list(List l)
{
	Cell p, q;
	for(p = l->ini, q = p->prox; q!=NULL; p = q, q = q->prox)
		free(p);
	free(p);
	free(l);
}


/*Rotinas Para fila circular*/

Fila filaInit() {

  Fila fila = mallocSafe(sizeof(*fila));

	data p;
	strcpy(p.nome, "cabeca");
	p.t0 = -2;
	p.dt = -2;
	p.deadline= -2;
	p.id = -2;
	Cell c = create_cell(p, NULL);
  Link cabeca = newNode(c, NULL, NULL);
  fila->cabeca = cabeca;
  fila->cabeca->next = cabeca;
  fila->cabeca->previous = cabeca;
  fila->current = cabeca;
  fila->last = cabeca;
  fila->n = 0;
  return fila;
}

void addProcessoFilaCircular(Cell p, Fila fila) {
                                /*Next     previous*/
  Link novo = newNode(p, fila->cabeca, fila->cabeca->previous);
  fila->cabeca->previous->next = novo;
  fila->cabeca->previous = novo;
  fila->n++;
  if (fila->current == fila->cabeca) {
    fila->current = fila->cabeca->next;
  }
}

/*Caso precise*/
// void imprimeFila (Fila fila, int i) {
//   Link no;
//   no = fila->cabeca->next;
//   while(no != fila->cabeca && no != NULL) {
//     imprimeProcesso(no->processo, i);
//     no = no->next;
//   }
// }

int filaSize(Fila fila) {
  return fila->n;
}

bool filaIsEmpty(Fila fila) {
  return fila->n == 0;
}

void retiraNodeFilaCircular(Fila fila) {
  Link no;
  if (fila->n == 0) {
    exit(1);
  }
  no = fila->current;
  fila->current->previous->next = fila->current->next;
  fila->current->next->previous = fila->current->previous;

  /*Se o próximo for a cabeca*/
  if (fila->current->next == fila->cabeca) {
    fila->current = fila->cabeca->next;
  } else {
    fila->current = fila->current->next;
  }
  fila->n--;
	free(no);
}

void proximoNode(Fila fila) {
	if (fila->current->next == fila->cabeca) {
		fila->current = fila->cabeca->next;
	} else {
		fila->current = fila->current->next;
	}
}


void filaFree(Fila fila) {
  Link no, q;
  if (fila->n != 0) {
    no = fila->cabeca->next;
    while(no != fila->cabeca && no != NULL) {
      q = no->next;
      freeNode(no);
      no = q;
    }
  }
  no = fila->cabeca;
  free(no);
  free(fila);
}


/* Implementação das rotinas auxiliares */
static void *mallocSafe(size_t nbytes) {
  void *p = malloc(nbytes);

  if (p == NULL) {
    printf("Erro: alocação de memória falhou no módulo Node.");
    exit(0);
  }
  return p;
}
