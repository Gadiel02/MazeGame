# Compilador que usaremos
CC = gcc
# Opciones de compilación (optimización y warnings)
CFLAGS = -Wall -Wextra -O2
# Librerías necesarias (SDL2 para gráficos y math para cálculos)
LDFLAGS = -lSDL2 -lm

# Archivos fuente de nuestro juego
SRCS = src/main.c src/game.c src/game_entities.c
# Convertir .c a .o
OBJS = $(SRCS:.c=.o)
# Nombre del programa final
TARGET = raycaster

.PHONY: all clean

# Comando principal: compila todo
all: $(TARGET)

# Enlaza los archivos objeto para crear el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compila cada archivo .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpia archivos temporales y el ejecutable
clean:
	rm -f $(OBJS) $(TARGET) src/*.o
