#include "../include/servidor.h"

int main(int argc, char *argv[]){
  /* Registrando rutina manejadora de señales */
  struct sigaction sa_sigint;
  struct sigaction sa_sigchld;

  //SIGINT se da cuando se presiona Ctl + C
  sa_sigint.sa_handler = sig_handler; //Se registra un manejador personalizado
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    perror("SIGINT");
    exit(-1);
  }
  //SIGCHLD: Enviada cuando un hijo termina. Esta señal será ignorada.
  //Gracias a la flag agregada SA_NOCLDWAIT los hijos que terminen no se
  //convertirán en zombies, es por esto que se ignora la señal.
  //ver: man 3 sigaction y man 3 wait
  sa_sigchld.sa_handler = SIG_IGN; //Se ignora la señal
  sa_sigchld.sa_flags = SA_NOCLDSTOP || SA_NOCLDWAIT;
  if(sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
  {
    perror("SIGCHLD");
    exit(-1);
  }

  /* Validación de parámetros */
  //Número de parámetros, se espera uno o dos parámetros
  if(argc > 2){
    usage();
    exit(-1);
  }
  //Si se enviaron dos argumentos
  if(argc == 2){
    //Si el segundo argumento es igual a "-h" se imprime el modo de uso
    if(!strcmp(argv[1], "-h")) //Iguales retorna 0
    {
      usage();
      exit(0);
    }
    //Se comprueba que el segundo argumento sea numérico.
    if (!(puerto=es_numero(argv[1])))
    {
      perror("[*] Mal argumento: El puerto debe ser un numero positivo.");
      exit(-1);
    }
  }
  else
  {
    //Si solo se recibe un paŕametro se establece el puerto por defecto
    puerto = 6666;
  }

  printf("\nIniciando servidor *** REVERSE EVIL SHELL ***\n");
  socket_des = crear_socket_servidor(puerto); //Se crea el socket escuchando el e lpuerto indicado
  if (socket_des == -1 ) //Se valida la creación correcta del socket
  {
    perror("Error abriendo el socket");
    return -1;
  }
  whoiam(); //Imprime la dirección ip y el puerto por el que se está escuchando
  int cliente_descriptor;
  do {
    //Escucha conexiones entrantes y retorna un descriptor para el cliente que se conecta
    cliente_descriptor = escuchar_clientes_nuevos();
    //Se divide el proceso actual, el proceso hijo atenderá al cliente conectado,
    //el padre voverá a la escuha
    switch(fork())
    {
      case -1:
	perror("Fork");
	break;
      case 0:
      {
	// Se convierte el cliente_descriptor en char* para pasarlo como parámetro al shell en execl
	char str_sokdes[10];
	snprintf(str_sokdes, 10, "%d", cliente_descriptor); //Se escribe el descriptor en el buffer
	// Se ejecuta el programa revil_shell ubicado en la misma ruta del servidor
	execl("./revil_shell", "./revil_shell", str_sokdes, (char*)NULL);
	perror("EXEC");
	exit(-1);
      }
	break;
    }
  }while(1); //El servidor se ejecuta indefinidamente
  return 0;
}

/*
#########################################
#					#
#		Métodos			#
#					#
#########################################
*/

/**
 * Crea un nuevo socket y llena la estructura sockaddr con la información necesaria.
 */
int crear_socket_servidor(int puerto)
{
  int sck = socket(AF_INET, SOCK_STREAM, 0); //Se crea un socket de red TCP
  struct sockaddr_in s_addin;
  s_addin.sin_family = AF_INET; //Dominio de red
  s_addin.sin_port = htons(puerto); //Se establece el puerto en orden de red.
  s_addin.sin_addr.s_addr = htonl(INADDR_ANY); //La ip será la del host donde se ejecute el servidor.
  bzero(&(s_addin.sin_zero),8); //Padding

  bind(sck,(struct sockaddr *)&s_addin, sizeof(struct sockaddr)); //Se registra el socket ante el sistema operativo.
  listen(sck, 10); //Se le indica al sistema operativo que está listo para recibir peticiones. máximo 10 concurrentes.
  return sck;
}

/**
 * Espera conexiones entrantes.
 * Retorna el descriptor del socket para la comunicación con el cliente conectado.
 */
int escuchar_clientes_nuevos(void)
{
  struct sockaddr_in host_cliente;
  int longitud = sizeof(struct sockaddr);
  //Espera peticiones entrantes a través del socket del servidor, la estructura sockaddr es
  //llenada con la información del cliente conectado.
  int cliente_des = accept(socket_des, (struct sockaddr *)(&host_cliente), &longitud);
  printf("[*] Conexión entrante...\n");
  if(cliente_des==-1) //Se valida la validez del socket.
  {
    perror("No se pudo establecer la conexión.");
    return -1;
  }
  //Imprime la dirección ip del cliente conectado. Obtiene la ip en network order gracias a la estructura
  //sockaddr y la transforma a char* con notación números y puntos.
  printf("[*] Establecida la conexión con: %s\n", inet_ntoa(host_cliente.sin_addr));
  return cliente_des;
}

/** Manejador de señales */
void sig_handler(int signal)
{
  switch(signal)
  {
    case SIGINT:
      //Se precionó Ctrl-C
      printf("[*] Apagando el servidor...\n");
      close(socket_des); //Se cierra el socket.
      exit(0);
      break;
    default: break;
  }
  return;
}

/** Imprime el uso de este programa. */
void usage(void)
{
  printf("uso:\nrevil_server [puerto]\n");
  printf("[puerto]: Es el puerto de escucha del servidor.\n");
}

/** Imprime la ip y el puerto de escucha del servidor. */
void whoiam(void)
{
  char nombre[100];
  struct hostent *he;
  if (gethostname(nombre, 100) == 0) //Retorna el nombre del host en el buffer nombre.
  {
    if((he=gethostbyname(nombre)) != NULL) //Retorna la estructura hostent del nombre del servidor dado.
    {
      //Se recupera en he la estructura hostent con la información de este servidor.
      //h_addr es el primer item en la lista de direcciones del servidor, se convierte a char*
      //en notación de puntos y se imprime.
      printf("El servidor está escuchando en %s : %d\n", inet_ntoa(*((struct in_addr *)he->h_addr)), puerto);
    }
  }
}

/** Verifica si el valor pasado es un número */
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