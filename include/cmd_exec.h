#ifndef __CMD_EXEC_H__
#define __CMD_EXEC_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "iosocket.h"

// Delimitador para hacer la tokenización
#define DELIM " "
//Máximo número de tokens permitidos para un comando
#define MAX_TOKENS 100

//Array donde se almacenarán los tokens
char *tokens[MAX_TOKENS];
//Socket usado para enviar la respuesta al servidor
extern int sckdes;

void exec_cmd(char* cmd);
void * enviar_respuesta(void* param);
int es_background(char* cmd);
#endif