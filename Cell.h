#include "Data.h"

/*Nó para a lista linear dos processos de entrada pelo arquivo*/
typedef struct cell *Cell;
struct cell
{
	data x;
	Cell prox;
};
/*Rotina do nó da lista linear dos processos*/
Cell create_cell(data, Cell);

/*Nó para a fila circular*/
typedef struct queueNode *Link;
struct queueNode {
  Link next;
  Link previous;
	Cell celula;
};

/*Rotinas dos nós da fila circular*/
Link newNode (Cell, Link, Link);
void freeNode (Link);
