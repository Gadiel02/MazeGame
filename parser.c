#include "raycaster.h"
#include <stdio.h>
#include <stdlib.h>

void load_map(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open map file %s\n", filename);
        exit(1);
    }
    
    // Inicializar mapa a ceros
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = 0;
        }
    }
    
    // Leer mapa del archivo
    char line[MAP_WIDTH + 2]; // +2 para \n y \0
    int y = 0;
    
    while (fgets(line, sizeof(line), file) && y < MAP_HEIGHT) {
        // Saltar líneas vacías
        if (line[0] == '\n') {
            continue;
        }
        
        for (int x = 0; x < MAP_WIDTH && line[x] != '\0' && line[x] != '\n'; x++) {
            if (line[x] == '1') {
                map[y][x] = 1;
            } else {
                map[y][x] = 0;
            }
        }
        y++;
    }
    
    fclose(file);
    
    // Imprimir mapa cargado para debug
    printf("Map loaded:\n");
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf("%d", map[y][x]);
        }
        printf("\n");
    }
}
