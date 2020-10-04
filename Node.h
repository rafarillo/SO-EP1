#include "Data.h"

typedef struct queueNode *Link;
struct queueNode {
  data x;
  Link next;
  Link previous;
  int idThread;
};

/*Cria Node com*/
Link newNode (data, Link, Link);
void freeNode (Link);
