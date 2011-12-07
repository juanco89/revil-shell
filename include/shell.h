#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>

#include "iosocket.h"

#define PROMPT "~~> "

void iniciar_shell(int cliente_conectado);
char * obtener_comando(char *buff);

#endif