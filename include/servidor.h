#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>

#include "iosocket.h"

/*Declaracion de variables globales*/
int socket_des;
int puerto;

/*Declaracion de prototipos de funciones*/
int crear_socket_servidor(int);
int escuchar_clientes_nuevos(void);
char* obtener_comando(char* buff);

#endif