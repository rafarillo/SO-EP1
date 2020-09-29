#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct info
{
	char nome[30];
	int t0, dt, deadline,tf;
};

typedef struct info data;

struct cell
{
	data x;
	struct cell *prox;
};

struct list
{
	struct cell *ini;
	int N;
};
typedef struct list List;

List * create_list();
void create_cell(data x,List *l);
struct cell *at(int i, List *l);
void dump_list();
void free_list(List *l);
int number_elements();