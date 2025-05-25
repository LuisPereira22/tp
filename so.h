#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <wait.h>

#define BUFFER_SIZE 1024


void mostra_ficheiro(const char *filename);

void copia_ficheiro(const char *filename);

void acrescenta_ficheiros(const char *origem, const char *destino);

void conta_linhas(const char *filename);

void apaga_ficheiro(const char *filename);

void informa_ficheiro(const char *filename);

void lista_diretoria(const char *dirname);

/* Funções da Parte 2 - Interpretador de Comandos */

int executar_comando(char *comando, char **argumentos);

void interpretador();

#endif 