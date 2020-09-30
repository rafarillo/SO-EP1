#include "Data.h"
typedef struct cell *Cell;
struct cell
{
	data x;
	Cell prox;
};

Cell create_cell(data, Cell);
