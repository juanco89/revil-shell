#ifndef __CLIENTE_H__
#define __CLIENTE_H__

#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#include <ctype.h>

#include "cmd_exec.h"

/*Declaracion de variables globales*/
int sckdes;
int puerto;
char* ip;

/*Declaracion de prototipos de funciones*/
/** puerto, host, struct sockaddr */
int crear_socket_cliente(int, char*, struct sockaddr *);
int conectar(int sck,struct sockaddr *addin);
void cerrar_cliente(void);
void sig_handler(int);
void usage(void);
int es_numero(char *valor);
int verificar_ip(char *ip);

#endif