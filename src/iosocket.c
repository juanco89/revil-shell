#include "../include/iosocket.h"

/**
 * descriptor de socket
 * buffer con los datos
 * longitud a enviar
 */
int escribir_socket(int sckdes, char* msg, int leng)
{
  return send(sckdes, msg, leng,0);
}

/**
 * descriptor de socket
 * buffer
 * longitud máxima posible
 */
int leer_socket(int sckdes, char* msg, int leng)
{
   return recv(sckdes,msg,leng,0);   
}