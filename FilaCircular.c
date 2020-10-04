#include "FilaCircular.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#define bool int

/* Protótipo de rotinas auxiliares */
static void *mallocSafe(size_t nbytes);


/*Rotinas Para fila circular*/

Fila filaInit() {
  Fila fila = mallocSafe(sizeof(*fila));
  data p;
  // p.nome = "cabeca";
  p.t0 = 0;
  p.dt = 0;
  p.deadline = 0;
  p.tf = 0;
  p.id = 0;
  p.idAnterior = 0;
  p.tr = 0;

  Link cabeca = newNode(p, NULL, NULL);
  fila->cabeca = cabeca;
  fila->cabeca->next = cabeca;
  fila->cabeca->previous = cabeca;
  fila->current = cabeca;
  fila->last = cabeca;
  fila->n = 0;
  return fila;
}

void addProcessoFilaCircular(data p, Fila fila) {
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

Link retiraNodeFilaCircular(Fila fila) {
  Link no;
  if (fila->n == 0) {
    return NULL;
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
  return no;
}

void proximoNode(Fila fila) {
  fila->current = fila->current->next;
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
