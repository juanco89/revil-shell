#include "../include/shell.h"

int main(int argc, char *argv[])
{
  //Registro de señales
  struct sigaction sa_sigint;
  struct sigaction sa_sigpipe;

  //Señal SIGINT
  sa_sigint.sa_handler = sig_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    perror("SIGINT");
    exit(-1);
  }
  //SIGPIPE: indica cuando el otro extremo de la conexión se perdió.
  //Usado para saber cuando se desconecta el cliente.
  sa_sigpipe.sa_handler = sig_handler;
  sa_sigpipe.sa_flags = SA_RESTART;
  if(sigaction(SIGPIPE, &sa_sigpipe, NULL) == -1)
  {
    perror("SIGPIPE");
    exit(-1);
  }
  //Se validan los argumentos. Se esperan dos argumentos nombre_prog descriptor_cliente
  if(argc != 2)
  {
    perror("Malos Argumentos");
    exit(-1);
  }
  //Se convierte el argumento 2 (descriptor_cliente) a entero.
  descriptor_cliente = atoi(argv[1]);
  //Se lanza un hilo para escuchar el socket. Imprime la respuesta de la 
  //ejecución de los comandos.
  pthread_t id_hilo;
  if(pthread_create(&id_hilo, NULL, escribir_respuesta, NULL) != 0){
    perror("THREAD");
    exit(-1);
  }
  iniciar_shell(); //Se inicia la shell
  terminar_shell(); //Se liberan recursos
  return 0;
}

/** Inicia una shell básica que recibe comandos desde la terminal y los envía
 * hacia el cliente para su ejecución.
 */
void iniciar_shell()
{
  // 1: indica que hay un cliente conectado.
  cliente_conectado = 1;

  int tam_comm;
  char * comando;
  int datos_enviados;
  while(cliente_conectado)
  {
    printf(PROMPT); //Imprime el prompt
    //fgets(comando, MAX_LINE, stdin);
    comando = obtener_comando(comando); //Obtiene la cadena leída por consola.
    tam_comm = strlen(comando); //strlen no cuenta el caracter nulo '\0'
    datos_enviados = escribir_socket(descriptor_cliente, comando, tam_comm + 1);
    //Libera la memoria pedida para el comando.
    free(comando);
  }
}

/**
 * lee una cadena desde la consola y la almacena en buff.
 * No lee más de MAX_LINE caracteres incluyendo el caracter '\0'.
 * Retorna el puntero hacia la cadena leída.
 */
char * obtener_comando(char *buff)
{
  //Pide memoria al SO para copiar la cadena leida.
  buff = (char *) malloc(sizeof(char) * MAX_LINE);
  char c;
  int i = 0;
  do{
    c = getchar(); //Lee uno a uno los caracteres.
    buff[i] = c;
    ++i;
    //La cadena se termina al leer salto de línea o al terminarse el espacio.
    //Deja un espacio para el caracter nulo '\0'
  }while(c != '\n' && i < (MAX_LINE - 1));
  //Si el último leído es un salto de línea devolvemos i para sobreescribir dicha posición.
  if(c == '\n')
  {
    i--;
  }
  //Se escribe el caracter nulo '\0'.
  buff[i] = '\0';
  return (buff);
}

/** Este método escucha el socket e imprime lo recibido. */
void * escribir_respuesta(void* args)
{
  char respuesta[MAX_LINE];
  //Mientras el cliente esté conectado.
  while(cliente_conectado)
  {
    //Lee el socket. 0 indica que el cliente se desconectó.
    if(leer_socket(descriptor_cliente, respuesta, MAX_LINE) == 0){
      cliente_conectado = 0; //Indica que el cliente ya no está conectado.
      break; //Se sale del ciclo.
    }else {
      printf("%s",respuesta); //Imprime lo recibido por el socket.
    }
  }
  pthread_exit((void *) 0);
}

/** Sirve para liberar recursos y terminar el proceso. */
void terminar_shell(void)
{
  cliente_conectado = 0;
  close(descriptor_cliente);
  exit(0);
}

/** Manejador de señales. */
void sig_handler(int signal)
{
  switch(signal)
  {
    case SIGINT:
      //Se precionó Ctrl-C
      printf(" Apagando el servidor...\n");
      terminar_shell(); //Termina el proceso.
      break;
    case SIGPIPE:
      printf("[x] Se ha perdido la conexión con el cliente\n\n");
      terminar_shell(); //Termina el proceso.
      break;
    default: break;
  }
  return;
}