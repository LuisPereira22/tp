/**
 * @file funcoes.c
 * @brief Conjunto de funções para manipulação de ficheiros e diretórios em C.
 */
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
#include "so.h"

#define BUFFER_SIZE 1024


/**
 * @brief Mostra o conteúdo de um ficheiro no terminal.
 * 
 * @param filename Nome do ficheiro a ser exibido.
 */
void mostra_ficheiro(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if(write(STDOUT_FILENO, buffer, bytes_read) != bytes_read){
            perror("Erro de escrita");
            break;
        }
    }
    if(bytes_read == -1)
    {
        perror("Erro de leitura");
    }

    close(fd);
}

/**
 * @brief Copia um ficheiro para outro com a extensão ".copia".
 * 
 * @param filename Nome do ficheiro de origem.
 */
void copia_ficheiro(const char *filename) {
    int src_fd = open(filename, O_RDONLY);
    if (src_fd == -1) {
        perror("Erro ao abrir ficheiro origem");
        return;
    }

    char dest_filename[256];
    snprintf(dest_filename, sizeof(dest_filename), "%s.copia", filename);
    
    int dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Erro ao criar ficheiro destino");
        close(src_fd);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("Erro ao escrever no ficheiro destino");
            break;
        }
    }

    close(src_fd);
    close(dest_fd);
}

/**
 * @brief Acrescenta o conteúdo de um ficheiro (origem) no fim de outro (destino).
 * 
 * @param origem Nome do ficheiro de origem.
 * @param destino Nome do ficheiro de destino.
 */
void acrescenta_ficheiros(const char *origem, const char *destino) {
    int src_fd = open(origem, O_RDONLY);
    if (src_fd == -1) {
        perror("Erro ao abrir ficheiro origem");
        return;
    }

    int dest_fd = open(destino, O_WRONLY | O_APPEND);
    if (dest_fd == -1) {
        perror("Erro ao abrir ficheiro destino");
        close(src_fd);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("Erro ao escrever no ficheiro destino");
            break;
        }
    }

    close(src_fd);
    close(dest_fd);
}

/**
 * @brief Conta o número de linhas de um ficheiro.
 * 
 * @param filename Nome do ficheiro a ser analisado.
 */
void conta_linhas(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    int count = 0;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Lê o arquivo linha por linha
    while ((bytes_read = read(fd, buffer, 1)) > 0) {
        if (bytes_read == 1 && buffer[0] == '\n') {
            count++;
        }
    }

    // Caso o arquivo tenha conteúdo e não termine com '\n', contamos a última linha
    if (bytes_read == 0 && count > 0 && buffer[bytes_read - 1] != '\n') {
        count++;
    }

    // Caso o arquivo tenha apenas uma linha sem final '\n'
    if (bytes_read > 0 && count == 0) {
        count = 1;
    }

    printf("Número de linhas: %d\n", count);
    close(fd);
}

/**
 * @brief Apaga um ficheiro do sistema de ficheiros.
 * 
 * @param filename Nome do ficheiro a ser apagado.
 */
void apaga_ficheiro(const char *filename) {
    if (unlink(filename) == -1) {
        perror("Erro ao apagar ficheiro");
    }
}

/**
 * @brief Mostra informações detalhadas sobre um ficheiro, incluindo tipo, i-node, dono e datas.
 * 
 * @param filename Nome do ficheiro a ser informado.
 */
void informa_ficheiro(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("Erro ao obter informação do ficheiro");
        return;
    }

    printf("Informação do ficheiro: %s\n", filename);
    
    // Tipo de ficheiro
    printf("Tipo: ");
    if (S_ISREG(file_stat.st_mode)) printf("Ficheiro regular\n");
    else if (S_ISDIR(file_stat.st_mode)) printf("Diretoria\n");
    else if (S_ISLNK(file_stat.st_mode)) printf("Link simbólico\n");
    else if (S_ISCHR(file_stat.st_mode)) printf("Dispositivo de caracteres\n");
    else if (S_ISBLK(file_stat.st_mode)) printf("Dispositivo de blocos\n");
    else if (S_ISFIFO(file_stat.st_mode)) printf("FIFO/pipe\n");
    else printf("Desconhecido\n");

    // i-node
    printf("i-node: %ld\n", (long)file_stat.st_ino);

    // Dono
    struct passwd *pwd = getpwuid(file_stat.st_uid);
    printf("Dono: %s\n", pwd ? pwd->pw_name : "Desconhecido");

    // Datas
    printf("Último acesso: %s", ctime(&file_stat.st_atime));
    printf("Última modificação: %s", ctime(&file_stat.st_mtime));
    printf("Última alteração de estado: %s", ctime(&file_stat.st_ctime));
}

/**
 * @brief Lista o conteúdo de uma diretoria especificada (ou a atual, se nulo).
 * 
 * @param dirname Nome da diretoria a listar (ou NULL para diretoria atual).
 */
void lista_diretoria(const char *dirname) {
    DIR *dir;
    struct dirent *entry;
    const char *path = dirname ? dirname : ".";

    dir = opendir(path);
    if (!dir) {
        perror("Erro ao abrir diretoria");
        return;
    }

    printf("Conteúdo da diretoria %s:\n", path);
    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) == -1) {
            perror("Erro ao obter informação do ficheiro");
            continue;
        }

        printf("%s - ", entry->d_name);
        if (S_ISREG(st.st_mode)) printf("Ficheiro\n");
        else if (S_ISDIR(st.st_mode)) printf("Diretoria\n");
        else if (S_ISLNK(st.st_mode)) printf("Link simbólico\n");
        else printf("Outro tipo\n");
    }

    closedir(dir);
}