#include "../include/shell.h"

void iniciar_shell(int cliente)
{
  descriptor_cliente = cliente;
  cliente_conectado = 1;

  pthread_t id_hilo;
  if(pthread_create(&id_hilo, NULL, escribir_respuesta, NULL) != 0){
    printf("Algo ha salido mal creando el hilo de escucha\n");
  }

  int tam_comm;
  char * comando;
  int datos_enviados;
  while(cliente_conectado)
  {
    printf(PROMPT);
    //fgets(comando, MAX_LINE, stdin);
    comando = obtener_comando(comando);
    tam_comm = strlen(comando); //strlen no cuenta el caracter nulo '\0'
    datos_enviados = escribir_socket(descriptor_cliente, comando, tam_comm + 1);
    free(comando);
  }
}

char * obtener_comando(char *buff)
{
  buff = (char *) malloc(sizeof(char) * MAX_LINE);
  char c;
  int i = 0;
  do{
    c = getchar();
    buff[i] = c;
    ++i;
  }while(c != '\n' && i < (MAX_LINE - 1));
  
  if(c == '\n')
  {
    //buff[i] = '\n';
    i--;
  }
  
  buff[i] = '\0';
  return (buff);
}

void * escribir_respuesta(void* args)
{
  char respuesta[MAX_LINE];
  while(cliente_conectado)
  {
    leer_socket(descriptor_cliente, respuesta, MAX_LINE);
    printf("%s",respuesta);
  }
  pthread_exit((void *) 0);
}

void terminar_shell(void)
{
  cliente_conectado = 0;
}