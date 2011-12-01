#include "../include/iosocket.h"

/**
 * descriptor de socket
 * buffer con los datos
 * longitud a enviar
 */
int escribir_socket(int sckdes, char* msg, int leng)
{
  int result = send(sckdes, msg, leng,0);
  return result;
}

/**
 * descriptor de socket
 * buffer
 * longitud máxima recivida
 */
int leer_socket(int sckdes, char* msg, int leng)
{
   int msg_len= recv(sckdes,msg,leng,0);
   return msg_len;
}
