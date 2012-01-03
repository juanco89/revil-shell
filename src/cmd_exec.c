#include "../include/cmd_exec.h"

int cantidad_tokens(char* cmd)
{
  int total = 0;
  char copy[MAX_LINE];
  strncpy(copy, cmd, strlen(cmd) + 1);
  char* token = strtok(copy, DELIM);
  while(token != NULL)
  {
    total++;
    token = strtok(NULL, DELIM);
  }
  return total;
}

char** tokenizador(char* cmd, int n_tokens)
{
  int size_p_char = sizeof(char*);
  char ** tokens = (char**) malloc(size_p_char * (n_tokens + 1));
  char cmd_copy[MAX_LINE];
  strncpy(cmd_copy, cmd, strlen(cmd) + 1);
  char* token;
  int i;
  token = strtok(cmd_copy, DELIM);
  for(i = 0; token != NULL; i = i + size_p_char, token = strtok(NULL, DELIM))
  {
    *(tokens + i) = token;
  }
  *(tokens + i) = (char *) NULL;
  return tokens;
}

void exec_cmd(char* cmd)
{
  int numero_tokens = cantidad_tokens(cmd);
  
  char ** argv = tokenizador(cmd, numero_tokens);
  
  pid_t pid_cmd;
  int ex_estado;
  pid_cmd = fork();
  switch(pid_cmd) 
  {
    case -1:
      perror("Ha fallado la creación del nuevo proceso\n");
      printf("Ha fallado la creación del nuevo proceso\n");
      break;
    case 0:
      printf("Este es el hijo\n");
      exit(0);
      break;
    default:
      wait(&ex_estado);
      printf("Este es el padre\n");
      break;
  }
  free(argv);
}

void esperar_hijos(void)
{
  
}