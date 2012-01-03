#ifndef __CMD_EXEC_H__
#define __CMD_EXEC_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "iosocket.h"

#define DELIM " "

int cantidad_tokens(char* cmd);
char** tokenizador(char* cmd, int n_tokens);
void exec_cmd(char * cmd);
void esperar_hijos(void);

#endif