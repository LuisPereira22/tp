# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Ficheiros-fonte
SRC = interpretador.c funcoes.c

# Header usado
HEADER = so.h

# Executável
TARGET = interpretador

# Regra principal
all: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Limpar
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
