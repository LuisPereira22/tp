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

/* Funções da Parte 1 - Manipulação de Ficheiros */

// Mostra o conteúdo de um ficheiro
void mostra_ficheiro(const char *filename);

// Cria uma cópia de um ficheiro
void copia_ficheiro(const char *filename);

// Acrescenta o conteúdo de um ficheiro a outro
void acrescenta_ficheiros(const char *origem, const char *destino);

// Conta o número de linhas de um ficheiro
void conta_linhas(const char *filename);

// Apaga um ficheiro
void apaga_ficheiro(const char *filename);

// Mostra informações sobre um ficheiro
void informa_ficheiro(const char *filename);

// Lista o conteúdo de uma diretoria
void lista_diretoria(const char *dirname);

/* Funções da Parte 2 - Interpretador de Comandos */

// Executa um comando externo
int executar_comando(char *comando, char **argumentos);

// Função principal do interpretador
void interpretador();

#endif /* INTERPRETADOR_H */