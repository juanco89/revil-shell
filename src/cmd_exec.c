#include "../include/cmd_exec.h"

void tokenizador(char* cmd)
{
  char copy[MAX_LINE];
  strncpy(copy, cmd, strlen(cmd) + 1);
  char *t;
  int i = 0;
  t = strtok(copy, DELIM);
  while(t != NULL && i < MAX_TOKENS - 1)
  {
    tokens[i] = t;
    i++;
    t = strtok(NULL, DELIM);
  }
  tokens[i]= (char *)NULL;
}

void exec_cmd(char* cmd)
{
  tokenizador(cmd);

  pid_t pid_cmd;
  int ex_estado=0;
  pid_cmd = fork();
  switch(pid_cmd)
  {
    case -1:
      perror("Ha fallado la creación del nuevo proceso\n");
      printf("Ha fallado la creación del nuevo proceso\n");
    case 0:
      execvp(tokens[0], tokens);
      printf("Proceso no ejecutado\n");
      exit(-1);
      break;
    default:
      wait(&ex_estado);
      printf("Finalizado el proceso hijo\n");
      break;
  }
}

void esperar_hijos(void)
{
  
}