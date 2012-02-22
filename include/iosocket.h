#ifndef __IOSOCKET_H__
#define __IOSOCKET_H__

#include <sys/socket.h>

#define MAX_LINE 100

int escribir_socket(int sckdes, char* msg, int leng);

int leer_socket(int sckdes,char* msg, int leng);

#endif