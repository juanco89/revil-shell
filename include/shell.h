#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "iosocket.h"

#define PROMPT "~~> "

/* Descriptor del cliente conectado. */
int descriptor_cliente;
/** Indica si un cliente está conectado.
 Se usa para controlar el ciclo del while y del hilo de escucha.*/
int cliente_conectado;

void iniciar_shell();
char * obtener_comando(char *buff);
void * escribir_respuesta(void* args);
void terminar_shell(void);
void sig_handler(int signal);
#endif