#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "process.h"

int main(int argc, char const *argv[])
{
	FILE *p = fopen(argv[2],"r");
	data processo;
	if(argc < 4)
	{
		printf("Numero invalido de argumentos\n");
		exit(1);
	}
	if(p == NULL) 
	{
		printf("Ocorreu erro na leitura\n");
		exit(1);
	}

	while(!feof(p))
		{
			fscanf(p,"%s %d %d %d",processo.nome,&processo.t0,&processo.dt,&processo.deadline);
			create_cell(processo);
		}


	fclose(p);
	dump_list();
	free_list();
	return 0;
}