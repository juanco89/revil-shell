#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "iosocket.h"

#define PROMPT "~~> "

int descriptor_cliente;
int cliente_conectado;

void iniciar_shell(int cliente);
char * obtener_comando(char *buff);
void * escribir_respuesta(void* args);
void terminar_shell(void);
#endif