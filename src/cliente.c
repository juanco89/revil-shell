#include "../include/cliente.h"

int main (int argc, char *argv[]){
  
  // Registro de los manejadores de señales
  struct sigaction sa_sigint;
  struct sigaction sa_sigchld;
  
  //Señal SIGINT: Cuando se presiona Ctl + C
  sa_sigint.sa_handler = sig_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) == -1)
  {
    perror("SIGINTC\n");
    exit(-1);
  }
  //SIGCHLD: Enviada cuando un hijo termina. Esta señal será ignorada.
  //Gracias a la flag agregada SA_NOCLDWAIT los hijos que terminen no se
  //convertirán en zombies, es por esto que se ignora la señal.
  //ver: man 3 sigaction y man 3 wait
  sa_sigchld.sa_handler = SIG_IGN;
  sa_sigchld.sa_flags = SA_NOCLDSTOP || SA_NOCLDWAIT;
  if(sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
  {
    perror("SIGCHLD");
    exit(-1);
  }
  
  //Evaluación de argumentos
  if(argc > 3 || argc == 1)
  {
    usage();
    exit(-1);
  }
  //Se verifica que la ip cumpla con el formato de números y puntos (xxx.xxx.xxx.xxx)
  if(!verificar_ip(argv[1]))
  {
    perror("Mal argumento: Se espera ip en formato números y puntos");
    exit(-1);
  }
  //Si pasa la validación se asigna la ip
  ip = argv[1];
  //El puerto puede ser ignorado y establecido uno por defecto
  if(argc == 3)
  {
    //Se verifica que el puerto sea un número
    if(!(puerto=es_numero(argv[2])))
    {
      perror("Mal argumento: El puerto debe ser numérico.\n");
      exit(-1);
    }
  }
  else
  {
    //Puerto por defecto
    puerto = 6666;
  }

  //Se configura la estructura sockaddr_in
  struct sockaddr_in addin;
  printf("Conectando al servidor...\n");
  sckdes=crear_socket_cliente(puerto, ip, (struct sockaddr *)&addin); //Se crea el socket
  //Se intenta conectar al serivodr por medio del socket obtenido.
  if(conectar(sckdes, (struct sockaddr *)&addin) != -1)
  {
    printf(">>> Conexion establecida con el servidor\n");
    char comando[MAX_LINE]; //Buffer para recibir el comando enviado desde el servidor.
    //Se lee lo enviado por el socket, 0 indica que la conección se cerró.
    //Un salto de línea en el servidor enviará el caracter '\0'.
    while(leer_socket(sckdes,comando,MAX_LINE) > 0)
    {
      if(*comando == '\0') continue; //Se omiten los saltos de línea.
      printf(">> %s\n",comando); //Se imprime el comando recibido.
      exec_cmd(comando); //Se ejecuta el comando.
    }
  }else
  {
   perror("No se ha podido conectar con el servidor\n");
  }
  //Se liberan recursos antes de terminar el proceso.
  cerrar_cliente();
}

/*
#########################################
#					#
#		Métodos			#
#					#
#########################################
*/

/**
 * Crea y retorna un socket para la comunicación con el servidor que escucha
 * en la ip y puerto indicados. La estructura sockaddr pasada se llena con los
 * datos del servidor al cual se desea conectarse.
 */ 
int crear_socket_cliente(int puerto, char* host, struct sockaddr *sock_client)
{
  // Se crea un socket de dominio Internet y tipo TCP
  int sck = socket(AF_INET, SOCK_STREAM, 0);
  //Se configura la estructura sockaddr con los datos del servidor al cual conectarse
  ((struct sockaddr_in*)sock_client)->sin_family = AF_INET; //Dominio internet
  ((struct sockaddr_in*)sock_client)->sin_port = htons(puerto); //Puerto en orden de red
  ((struct sockaddr_in*)sock_client)->sin_addr.s_addr = inet_addr(host); //Ip del servidor en orden de red
  bzero(&(((struct sockaddr_in*)sock_client)->sin_zero),8); //Padding
  return sck;
}

/**Conectar al host especificado por addin por medio del socket
 sck, retorna 0 si la conexion es exitosa, -1 de lo contrario*/
int conectar(int sck,struct sockaddr *addin)
{
    int conexion=connect(sck, addin, sizeof(struct sockaddr));
    return conexion;
}

/** Cierra el socket */
void cerrar_cliente()
{
  printf(" Se cierra la conexión\n");
  close(sckdes); //Se cierra el socket
  exit(0); //Se cierra el proceso
}

/** Manejador de señales */
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

/** Imprime la forma de uso de este programa */
void usage(void)
{
  printf("Uso:\nrevil_client [ip] [puerto]\n");
  printf("[ip]: Es la ip del servidor al cual se conectará.\n");
  printf("[puerto]: Puerto de escucha del servidor.\n");
}

/** Verifica si el char* pasado es un numero */
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

/** Verifica si la ip pasada como argumento tiene el formato correcto. */
int verificar_ip(char *ip)
{
  int i, nums = 0, puntos = 0;
  for(i = 0; ip[i] != '\0'; i++)
  {
    if(isdigit(ip[i]))
    {
      nums++;
      //No puede haber más de 3 digitos seguidos
      if(nums > 3) return 0;
      continue;
    }
    if(ip[i] == '.')
    {
      //No pueden haber dos puntos seguidos
      if(nums == 0) return 0;
      puntos++;
      //No pueden haber en total más de tres puntos (¿?)
      if(puntos > 3) return 0;
      nums = 0;
      continue;
    }
    return 0;
  }
  //Al finalizar el recorrido no pueden haber menos de 3 puntos y la variable nums
  //no puede ser 0, esto indicaría que despúes del último punto no hubo números
  if(puntos < 3 || nums == 0)return 0;
  return 1;
}