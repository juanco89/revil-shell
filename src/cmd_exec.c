#include "../include/cmd_exec.h"

void exec_cmd(char* cmd)
{
  int p[2];
  pipe(p);

  int bg = es_background(cmd);

  pid_t pid_cmd;
  pid_cmd = fork();
  switch(pid_cmd)
  {
    case -1:
      perror("Ha fallado la creación del nuevo proceso\n");
      break;
    case 0:
    {
      //Tokenizar el comando recibido para pasarlo como array a execvp
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

      for(i=0;tokens[i]!=NULL;i++)printf("%s\n", tokens[i]);
      
      close(1); //stdout: 1
      dup(p[1]); // Se duplica el extremo de escritura de la pipe en stdout.
      close(p[0]); // Cerrar el extremo de lectura (el hijo no lo usará)

      execvp(tokens[0], tokens);
      perror("Exec");
      exit(-1);
    }
      break;
    default:
      close(p[1]); //Cerrar pipe de escritura.
      pthread_t hilo_id;
      pthread_create(&hilo_id, NULL, enviar_respuesta, (void*)p);
      //El padre espera la finalización del hijo, almenos de que se deba ejecutar en background
      if(!bg)
      {
	wait(NULL);
	pthread_join(hilo_id, NULL);
      }
      break;
  }
}

void * enviar_respuesta(void* param)
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
  close(p_write);
  pthread_exit((void *) NULL);
}

int es_background(char* cmd)
{
  int l = strlen(cmd);
  if(cmd[l-1]=='&')
  {
    cmd[l -1] = '\0';
    return 1;
  }
  return 0;
}