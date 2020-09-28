#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

void create_cell(data x)
{
	list *new_node = malloc(sizeof(list));
	strcpy(new_node->x.nome,x.nome);
	new_node->x.t0 = x.t0;
	new_node->x.dt = x.dt;
	new_node->x.deadline = x.deadline;

	if(ini == NULL) ini = new_node;
	else
	{
		list *p;
		for(p = ini; p->prox != NULL; p = p->prox);
		p->prox = new_node;
	}
	N++;
}

void dump_list()
{
	for(list *p = ini; p != NULL; p = p->prox)
		printf("%s %d %d %d\n",p->x.nome,p->x.t0,p->x.dt,p->x.deadline);
}

void free_list()
{
	list *p,*q;
	for(p = ini, q = p->prox; q!=NULL; p = q, q = q->prox)
		free(p);
	free(p);
}

int number_elements()
{
	return N;
}