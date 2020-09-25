#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct processo
{
	char nome[5];
	int t0, dt, deadline;

}Processo;

int main(int argc, char const *argv[])
{
	FILE *p = fopen(argv[2],"r");
	char line[100];
	Processo *ps;
	ps = malloc(10*sizeof(Processo));
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
	while(fgets(line,100,p) != NULL)
	{
		printf("%s",line);

	}
}