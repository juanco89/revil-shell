#ifndef __CLIENTE_H__
#define __CLIENTE_H__

#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "iosocket.h"

/*Declaracion de variables globales*/
int sckdes;
int puerto;
char* ip;

/*Declaracion de prototipos de funciones*/
/** puerto, host, struct sockaddr */
int crear_socket_cliente(int, char*, struct sockaddr *);
void escribir_respuesta(char*);
int conectar(int sck,struct sockaddr *addin);
void cerrar_cliente(void);
void sig_handler(int);

#endif