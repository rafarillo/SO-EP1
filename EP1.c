#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h> //Para o getcwd
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para o PATH_MAX

/*Função que printa o user e o PATH; e lê o que o usuario digita */
char* type_prompt(char* usuario)
{
	char *line;
	line = readline(usuario);
	//printf("%s",line);
	return line;
}

char** comando(char *line, int * args)
{
	char **comando,*c;
	comando = malloc(5*sizeof(char*));
	int arg = 0;
	c = strtok(line," ");
	for(int i = 0; i < 5; i++)
		comando[i] = NULL;
	comando[arg++] = c;
	//printf("%s\n",comando[0]);
	while((c =strtok(NULL," ")) != NULL )
	{
		comando[arg] = c;
		//printf("%s\n",comando[arg]);
		arg++;
	}
	*args = arg;
	return comando;
}

int main()
{
  /*Pegando o o diretorio atual*/
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd() error");
    return 1;
  }
  /*Pegando o nome de usuário*/
  char *user = getenv("USER");

  /*Vetor 'usuario' que indica o user e o diretorio atual*/
  int tam = 5 + strlen(cwd)+strlen(user);
  char usuario[tam];
  snprintf(usuario, tam, "{%s@%s} ", user, cwd);

	char *line,**commands;
	int args;
	while(1)
	{
    /*Acrescentei o vetor 'usuario' na chamada do prompt*/
		line = type_prompt(usuario);
		//commands = comando(line,&args);
		//for(int i = 0; commands[i] != NULL; i++) printf("%s\n",commands[i]);

		printf("%s\n",line);
		//for(int i = 0; i < args; i++) free(commands[i]);
		//printf("%s\n",a);
	}
}
