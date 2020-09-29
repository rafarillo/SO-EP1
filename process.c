#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

List *create_list()
{
	List *l = malloc(sizeof(List));
	l->ini = NULL;
	l->N = 0;
	return l;
}

void create_cell(data x,List *l)
{
	struct cell *new_node = malloc(sizeof(struct cell));
	strcpy(new_node->x.nome,x.nome);
	new_node->x.t0 = x.t0;
	new_node->x.dt = x.dt;
	new_node->x.deadline = x.deadline;
	new_node->x.tf = 0;

	if(l->ini == NULL) l->ini = new_node;
	else
	{
		struct cell *p;
		for(p = l->ini; p->prox != NULL; p = p->prox);
		p->prox = new_node;
	}
	l->N++;
}

struct cell *at(int i, List *l)
{
	struct cell *p;
	int k;
	for(k = 0, p = l->ini; k < i; p=p->prox,k++);
	return p;
}

void dump_list(List *l)
{
	for(struct cell *p = l->ini; p != NULL; p = p->prox)
		printf("%s %d %d %d\n",p->x.nome,p->x.t0,p->x.dt,p->x.deadline);
}

void free_list(List *l)
{
	struct cell *p,*q;
	for(p = l->ini, q = p->prox; q!=NULL; p = q, q = q->prox)
		free(p);
	free(p);
	free(l);
}
