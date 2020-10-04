#include "Data.h"
typedef struct cell *Cell;
struct cell
{
	data x;
	Cell prox;
};

Cell create_cell(data, Cell);

typedef struct queueNode *Link;
struct queueNode {
  Link next;
  Link previous;
	Cell celula;
};

/*Cria Node com*/
Link newNode (Cell, Link, Link);
void freeNode (Link);
