#include "../include/shell.h"

int main(int argc, char *argv[])
{
  struct sigaction sa_sigint;
  struct sigaction sa_sigpipe;
  struct sigaction sa_sigchld;

  sa_sigint.sa_handler = sig_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    perror("SIGINIT");
    exit(-1);
  }
  sa_sigpipe.sa_handler = sig_handler;
  sa_sigpipe.sa_flags = SA_RESTART;
  if(sigaction(SIGPIPE, &sa_sigpipe, NULL) == -1)
  {
    perror("SIGPIPE");
    exit(-1);
  }
  
  pthread_t id_hilo;
  if(pthread_create(&id_hilo, NULL, escribir_respuesta, NULL) != 0){
    perror("THREAD");
    exit(-1);
  }
  
  if(argc != 2)
  {
    perror("Malos Argumentos");
    exit(-1);
  }
  descriptor_cliente = atoi(argv[1]);
  iniciar_shell();
  terminar_shell();
  return 0;
}

void iniciar_shell()
{
  cliente_conectado = 1;

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
    if(leer_socket(descriptor_cliente, respuesta, MAX_LINE) == 0){
      break;
    }else {
      printf("%s",respuesta);
    }
  }
  pthread_exit((void *) 0);
}

void terminar_shell(void)
{
  cliente_conectado = 0;
  close(descriptor_cliente);
  exit(0);
}

void sig_handler(int signal)
{
  switch(signal)
  {
    case SIGINT:
      //Se precionó Ctrl-C
      printf(" Apagando el servidor...\n");
      terminar_shell();
      //exit(0);
      break;
    case SIGPIPE:
      printf("[x] Se ha perdido la conexión con el cliente\n\n");
      terminar_shell();
      break;
    default: break;
  }
  return;
}