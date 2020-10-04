#include "Node.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Protótipo de rotinas auxiliares */

static void *mallocSafe(size_t nbytes);

Link newNode (data p, Link next, Link previous) {
  Link new_node = mallocSafe(sizeof(*new_node));
  strcpy(new_node->x.nome,p.nome);
  new_node->x.t0 = p.t0;
	new_node->x.dt = p.dt;
	new_node->x.deadline = p.deadline;
	new_node->x.tf = 0;
	new_node->x.id = p.id;
	new_node->x.idAnterior = -2;
  new_node->idThread = p.id;
  new_node->next = next;
  new_node->previous = previous;
  return new_node;
}

void freeNode (Link q) {
  q->next = NULL;
  q->previous = NULL;
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
