####
#
# Archivo: Makefile
#
# Para compilar todo el proyecto, hágalo con la orden make.
# Para compilar solo el servidor: make servidor
# Para compilar solo el cliente: make cliente
# Para limpiar: make clean
# Para limpiar todo (con ejecutables): make clean_all
####


###Direcotorios de la aplicación
INCLUDEDIR = ./include
LIBDIR = ./lib
BINDIR = ./bin
SRCDIR = ./src

###Programas estándar que se utilizan
AR = ar
AFLAGS = rs
DBG=gdb

###Compilador
CC = gcc
CFLAGS = -I$(INCLUDEDIR)
LFLAGS = -L$(LIBDIR)
CLIBS = -lpthread
DEBUG = -g$(DBG)


###Biblioteca de usuario y archivos que forman parte de la biblioteca
BIBLIOTECA= $(LIBDIR)/librevil.a
OBJETOS_BIBLIOTECA = iosocket.o


###Programa ejecutable
SERVIDOR = $(BINDIR)/revil_server
OBJETOS_SERVIDOR = $(SRCDIR)/{iosocket.c,servidor.c,shell.c}

CLIENTE = $(BINDIR)/revil_client
OBJETOS_CLIENTE = $(SRCDIR)/{iosocket.c,cliente.c,cmd_exec.c}

ALL = $(SERVIDOR) $(CLIENTE)


###Tuplas
all : $(ALL)

servidor: $(SERVIDOR)

cliente: $(CLIENTE)

$(SERVIDOR):
	echo " [*]Generando el servidor"
	$(CC) $(OBJETOS_SERVIDOR) $(CFLAGS) $(CLIBS) $(DEBUG) -o $(SERVIDOR)
	echo " --> Listo!"

$(CLIENTE):
	echo " [*]Generando el cliente"
	$(CC) $(OBJETOS_CLIENTE) $(CFLAGS) $(DEBUG) -o $(CLIENTE)
	echo " --> Listo!"

## Regla no usada
$(BIBLIOTECA) : $(OBJETOS_BIBLIOTECA)
	echo " [*]Construyendo la biblioteca"
	$(AR) $(AFLAGS) $(BIBLIOTECA) $(OBJETOS_BIBLIOTECA)
	echo " --> Listo!"

# Regla para borrar
clean :
	echo " [*]Eliminando archivos objetos y bibliotecas..."
	rm -rf *.o *.a
	echo " --> Hecho"
#Regla para borrar todo
clean_all:
	echo " [*]Eliminando archivos objeto, bibliotecas y ejecutables..."
	rm -rf *.o *.a $(BINDIR)/*
	echo " --> Hecho"

###########################################################
#SRC = $(shell ls *.c)
#OBJECTS = $(SRC:.c=.o)

# Regla que genera el archivo ejecutable
#ejecutable: $(OBJECTS)
#	echo "generando el archivo ejecutable..."
#	gcc $(LFLAGS) $^ -o $@

# Reglas para generar los archivos objeto del programa
#%.o : %.c
#	echo "generando los archivos objeto..."
#	$(CC) $(CFLAGS) $< -o $@