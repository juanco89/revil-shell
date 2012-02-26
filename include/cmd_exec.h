#ifndef __CMD_EXEC_H__
#define __CMD_EXEC_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "iosocket.h"

#define DELIM " "
#define MAX_TOKENS 100

char *tokens[MAX_TOKENS];
extern int sckdes;

void tokenizador(char* cmd);
void exec_cmd(char* cmd);
void * enviar_respuesta(void* param);
int es_background(char* cmd);
#endif