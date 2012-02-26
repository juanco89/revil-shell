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

  int p[2];
  pipe(p);

  pid_t pid_cmd;
  pid_cmd = fork();
  switch(pid_cmd)
  {
    case -1:
      perror("Ha fallado la creación del nuevo proceso\n");
      break;
    case 0:
      close(p[0]); // Cerrar el extremo de lectura (el hijo no lo usará)
      close(1); //stdout: 1
      dup(p[1]); // Se duplica el extremo de escritura de la pipe en stdout.

      printf("%d\n", stdout); // <<---  DEBUG
      
      close(p[1]);

      execvp(tokens[0], tokens);
      perror("");
      exit(-1);
      break;
    default:
      close(p[1]); // Cerrar pipe de escritura.
      pthread_t hilo_id;
      pthread_create(&hilo_id, NULL, enviar_respuesta, (void*)p);
      wait(NULL);
      pthread_join(hilo_id, NULL);
      close(p[0]);
      break;
  }
}

void * enviar_respuesta(void * param)
{
  int p_write = (int) ((int*)param)[0];
  FILE *f = fdopen(p_write, "r");
  char buffer[MAX_LINE];
  int leido;
  while((leido=fread(buffer, 1, MAX_LINE, f)) != 0) {
    printf("%s\n",buffer);
    escribir_socket(sckdes, buffer, leido);
  }
  fclose(f);
  pthread_exit((void *) NULL);
}