#include "../include/servidor.h"

int main(int argc, char *argv[]){
  /* Registrando rutina manejadora de señales */
  struct sigaction sa_sigint;
  struct sigaction sa_sigchld;

  sa_sigint.sa_handler = sig_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    perror("SIGINT");
    exit(-1);
  }
  sa_sigchld.sa_handler = SIG_IGN;
  sa_sigchld.sa_flags = SA_NOCLDSTOP || SA_NOCLDWAIT;
  if(sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
  {
    perror("SIGCHLD");
    exit(-1);
  }

  /* Validación de parámetros */
  if(argc > 2){
    usage();
    exit(-1);
  }
  if(argc == 2){
    if(!strcmp(argv[1], "-h"))
    {
      usage();
      exit(0);
    }
    if (!(puerto=es_numero(argv[1])))
    {
      perror("[*] Mal argumento: El puerto debe ser un numero positivo.");
      exit(-1);
    }
  }
  else
  {
    // Puerto por defecto
    puerto = 6666;
  }

  printf("\nIniciando servidor reverse evil shell...\n");
  socket_des = crear_socket_servidor(puerto);
  if (socket_des == -1 )
  {
    perror("Error abriendo el socket");
    return -1;
  }
  whoiam();
  printf(">>> El servidor reverse evil shell está escuchando.\n\n");
  int cliente_descriptor;
  do {
    cliente_descriptor = escuchar_clientes_nuevos();

    switch(fork())
    {
      case -1:
	perror("Fork");
	break;
      case 0:
      {
	// Se convierte el cliente_descriptor es char * para pasarlo como parámetro al shell
	char str_sokdes[10];
	snprintf(str_sokdes, 10, "%d", cliente_descriptor);
	// Se ejecuta el programa revil_shell ubicado en la misma ruta del servidor
	execl("./revil_shell", "./revil_shell", str_sokdes, (char*)NULL);
	perror("EXEC");
	exit(-1);
      }
	break;
    }
  }while(1);
  return 0;
}

/*
#########################################
#					#
#		Métodos			#
#					#
#########################################
*/

int crear_socket_servidor(int puerto)
{
  int sck = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in s_addin;
  s_addin.sin_family = AF_INET;
  s_addin.sin_port = htons(puerto);
  s_addin.sin_addr.s_addr = htonl(INADDR_ANY);
  bzero(&(s_addin.sin_zero),8);

  bind(sck,(struct sockaddr *)&s_addin, sizeof(struct sockaddr));
  listen(sck, 10);
  return sck;
}

int escuchar_clientes_nuevos(void)
{
  struct sockaddr_in host_cliente;
  int longitud = sizeof(struct sockaddr);
  int cliente_des = accept(socket_des, (struct sockaddr *)(&host_cliente), &longitud);
  printf("Conexión entrante...\n");
  if(cliente_des==-1)
  {
    perror("No se pudo establecer la conexión.");
    return -1;
  }
  printf(">>> Establecida la conexión con: %s\n", inet_ntoa(host_cliente.sin_addr));
  return cliente_des;
}

void sig_handler(int signal)
{
  switch(signal)
  {
    case SIGINT:
      //Se precionó Ctrl-C
      printf(" Apagando el servidor...\n");
      close(socket_des);
      exit(0);
      break;
    default: break;
  }
  return;
}

void usage(void)
{
  printf("uso:\nrevil_server [puerto]\n");
  printf("[puerto]: Es el puerto de escucha del servidor.\n");
}

void whoiam(void)
{
  char nombre[100];
  struct hostent *he;
  if (gethostname(nombre, 100) == 0)
  {
    if((he=gethostbyname(nombre)) != NULL)
    {
      printf("Dirección IP del servidor %s\n", inet_ntoa(*((struct in_addr *)he->h_addr)));
    }
  }
}

int es_numero(char *valor)
{
  int i;
  for(i = 0; valor[i] != '\0'; i++)
  {
    if(!isdigit(valor[i]))
      return 0;
  }
  return atoi(valor);
}
