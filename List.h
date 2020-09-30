#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cell.h"

typedef struct list *List;
struct list
{
	Cell ini;
	int N;
};

List create_list();
void addCell(data x,List l);
Cell  at(int i, List l);
void dump_list();
void free_list(List l);
int number_elements();
