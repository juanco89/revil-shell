#ifndef __IOSOCKET_H__
#define __IOSOCKET_H__

#include <sys/socket.h>

// Máxima longitud que será transmitida a través de un socket.
#define MAX_LINE 200

int escribir_socket(int sckdes, char* msg, int leng);

int leer_socket(int sckdes,char* msg, int leng);

#endif