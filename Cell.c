#include "Cell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Protótipo de rotinas auxiliares */
static void *mallocSafe(size_t nbytes);

Cell create_cell(data p, Cell proxima)
{
	Cell new_node = mallocSafe(sizeof(*new_node));
	strcpy(new_node->x.nome,p.nome);
	new_node->x.t0 = p.t0;
	new_node->x.dt = p.dt;
	new_node->x.deadline = p.deadline;
	new_node->x.tf = 0;
	new_node->x.id = p.id;
	new_node->x.idAnterior = -2;
	new_node->x.tquantum = 0;
  return new_node;
}

Link newNode (Cell processo, Link next, Link previous) {
	Link new_node = mallocSafe(sizeof(*new_node));
	new_node->celula = processo;
  new_node->next = next;
  new_node->previous = previous;
  return new_node;
}

void freeNode (Link q) {
  q->next = NULL;
  q->previous = NULL;
	q->celula = NULL;
  free(q);
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
