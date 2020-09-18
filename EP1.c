#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
char cwd[PATH_MAX];

char *usuario() {
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
      // printf("Current working dir: %s\n", cwd);
      return cwd;
  } 
  else {
      perror("getcwd() error");
      return "";
  }
}

char *type_prompt()
{
	char *line;
	line = readline(usuario());
	//printf("%s",line);
	return line;
}

char **comando(char *line, int * args)
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
	char *line,**commands;
	int args;
	while(1)
	{
		line = type_prompt();
		//commands = comando(line,&args);
		//for(int i = 0; commands[i] != NULL; i++) printf("%s\n",commands[i]);

		printf("%s\n",line);
		//for(int i = 0; i < args; i++) free(commands[i]);
		//printf("%s\n",a);
	}
}