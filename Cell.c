#include "Cell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Cell create_cell(data p, Cell proxima)
{
	Cell new_node = malloc(sizeof(*new_node));
	strcpy(new_node->x.nome,p.nome);
	new_node->x.t0 = p.t0;
	new_node->x.dt = p.dt;
	new_node->x.deadline = p.deadline;
	new_node->x.tf = 0;
	new_node->x.id = p.id;
	new_node->x.idAnterior = -2;
  return new_node;
}
