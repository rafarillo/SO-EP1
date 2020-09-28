#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct info
{
	char nome[30];
	int t0, dt, deadline;
};

typedef struct info data;

struct cell
{
	data x;
	struct cell *prox;
};

typedef struct cell list;

static list *ini = NULL;
static int N = 0;

void create_cell(data x);
void dump_list();
void free_list();
int total_process();