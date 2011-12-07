#include "../include/shell.h"

void iniciar_shell(int descriptor_cliente)
{
  while(descriptor_cliente)
  {
    printf(PROMPT);
    
  }
}

char * obtener_comando(char *buff)
{
  buff = (char *) malloc(sizeof(char) * MAX_LINE);
  char c;
  int i = 0;
  do{
    c = getchar();
    buff[i] = c;
    ++i;
  }while(c != '\n' && i < (MAX_LINE - 2));
  
  if(c != '\n')
  {
    buff[i] = '\n';
  }
  ++i;
  buff[i] = '\0';
  return (buff);
}