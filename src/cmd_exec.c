#include "../include/cmd_exec.h"

/**
 * Se encarga de la ejecución del comando recibido. Tokeniza el comando para crear
 * un array con los parámetros y así pasarlo a execvp.
 * Se encarga de hacer el fork y redireccionar la respuesta hacia el servidor.
 */ 
void exec_cmd(char* cmd)
{
  //Pipe para redirigir la salida del comando ajecutado hacia el servidor.
  int p[2];
  pipe(p);

  // 1 si debe ejecutarse en background, 0 de lo contario.
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
      char *t;
      int i = 0;
      //Retorna un apuntador al primer token de cmd delimitado por DELIM (" ")
      t = strtok(cmd, DELIM);
      while(t != NULL && i < MAX_TOKENS - 1) // Se guarda 1 posicion para (char*)NULL
      {
	tokens[i] = t;
	i++;
	// Los siguientes tokens se retornan indicando NULL
	t = strtok(NULL, DELIM);
      }
      // Es necesario que la última posición del array de parámetros sea NULL.
      tokens[i]= (char *)NULL;

      //Para capturar la salida y enviarla al servidor.
      //Se usa un pipe, y se establecerá el descriptor de escritura de este como la salida estándar.
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

/**
 * Este método se ejecuta en un hilo y es el encargado de enviar la respuesta
 * de la ejecución de un comando hacia el servidor. La respuesta es leida de
 * una pipe, al llamarlo es pasado su descriptor de lectura.
 */
void * enviar_respuesta(void* param)
{
  //Se obtiene el parámetro, este es el descriptor del extremo de lectura de la pipe
  int p_write = (int) ((int*)param)[0];
  //Buffer para recibir la respuesta
  char buffer[MAX_LINE];
  int leido;
  // Se lee desde el descriptor no mas de MAX_LINE y se almacena
  //en buffer, si se retorna 0 es porque se llegó al final del archivo
  //o porque se ha cerrado la conexión con el servidor.
  while((leido=read(p_write, buffer, MAX_LINE)) != 0) {
    escribir_socket(sckdes, buffer, leido);
  }
  //Se cierra el descriptor de la pipe
  close(p_write);
  //Se Termina el hilo, no se retorna nada
  pthread_exit((void *) NULL);
}

/**
 * Indica si el comando debe ejecutarse en background.
 */
int es_background(char* cmd)
{
  int l = strlen(cmd);
  if(cmd[l-1]=='&')
  {
    cmd[l-1] = '\0';
    return 1;
  }
  return 0;
}