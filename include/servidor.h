#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <ctype.h>

#include "shell.h"

/*Declaracion de variables globales*/
int socket_des;
int puerto;

/*Declaracion de prototipos de funciones*/
int crear_socket_servidor(int puerto);
int escuchar_clientes_nuevos(void);
void sig_handler(int signal);
void usage(void);
void whoiam(void);
int es_numero(char *valor);

#endif