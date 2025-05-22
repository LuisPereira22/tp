#include "so.h"
#include <stdbool.h>

// Função para executar um comando
int executar_comando(char *comando, char **argumentos) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Erro ao criar processo filho");
        return -1;
    } else if (pid == 0) {
        // Processo filho
        execvp(comando, argumentos);
        perror("Erro ao executar comando");
        exit(EXIT_FAILURE);
    } else {
        // Processo pai
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}

// Interpretador principal
void interpretador() {
    char linha[1024];
    char *comando = NULL;
    char *argumentos[64]; // Máximo de 64 argumentos
    int num_argumentos;
    
    printf("Interpretador de comandos (digite 'termina' para sair)\n");
    
    while (1) {
        printf("%% ");
        fflush(stdout);
        
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            break; // EOF (Ctrl+D)
        }
        
        // Remover newline
        linha[strcspn(linha, "\n")] = '\0';
        
        // Verificar comando vazio
        if (strlen(linha) == 0) {
            continue;
        }
        
        // Tokenizar a linha
        num_argumentos = 0;
        comando = strtok(linha, " ");
        
        if (comando == NULL) {
            continue;
        }
        
        // Verificar comando de saída
        if (strcmp(comando, "termina") == 0) {
            break;
        }
        
        // Verificar se é um dos nossos comandos implementados
        if (strcmp(comando, "mostra") == 0) {
            char *filename = strtok(NULL, " ");
            if (filename) {
                mostra_ficheiro(filename);
            } else {
                fprintf(stderr, "Uso: mostra ficheiro\n");
            }
            continue;
        } else if (strcmp(comando, "copia") == 0) {
            char *filename = strtok(NULL, " ");
            if (filename) {
                copia_ficheiro(filename);
            } else {
                fprintf(stderr, "Uso: copia ficheiro\n");
            }
            continue;
        } else if (strcmp(comando, "acrescenta") == 0) {
            char *origem = strtok(NULL, " ");
            char *destino = strtok(NULL, " ");
            if (origem && destino) {
                acrescenta_ficheiros(origem, destino);
            } else {
                fprintf(stderr, "Uso: acrescenta origem destino\n");
            }
            continue;
        } else if (strcmp(comando, "conta") == 0) {
            char *filename = strtok(NULL, " ");
            if (filename) {
                conta_linhas(filename);
            } else {
                fprintf(stderr, "Uso: conta ficheiro\n");
            }
            continue;
        } else if (strcmp(comando, "apaga") == 0) {
            char *filename = strtok(NULL, " ");
            if (filename) {
                apaga_ficheiro(filename);
            } else {
                fprintf(stderr, "Uso: apaga ficheiro\n");
            }
            continue;
        } else if (strcmp(comando, "informa") == 0) {
            char *filename = strtok(NULL, " ");
            if (filename) {
                informa_ficheiro(filename);
            } else {
                fprintf(stderr, "Uso: informa ficheiro\n");
            }
            continue;
        } else if (strcmp(comando, "lista") == 0) {
            char *dirname = strtok(NULL, " ");
            lista_diretoria(dirname);
            continue;
        }
        
        // Se não for um dos nossos comandos, tratar como comando do sistema
        argumentos[0] = comando;
        num_argumentos = 1;
        
        char *arg;
        while ((arg = strtok(NULL, " ")) != NULL && num_argumentos < 63) {
            argumentos[num_argumentos++] = arg;
        }
        argumentos[num_argumentos] = NULL;
        
        int resultado = executar_comando(comando, argumentos);
        printf("Terminou comando %s com código %d\n", comando, resultado);
    }
}

int main() {
    interpretador();
    return 0;
}