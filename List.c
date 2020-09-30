#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List create_list()
{
	List l = malloc(sizeof(List));
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
