#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#include "iosocket.h"

/*Declaracion de variables globales*/
int socket_des;
int puerto;
int cliente_conectado;

/*Declaracion de prototipos de funciones*/
int crear_socket_servidor(int puerto);
int escuchar_clientes_nuevos(void);
char* obtener_comando(char* buff);
void sig_handler(int signal);

#endif