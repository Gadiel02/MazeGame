
#include "raycaster.h"

bool check_collision(float x, float y) {
    int map_x = (int)(x / TILE_SIZE);
    int map_y = (int)(y / TILE_SIZE);

    // Verificar límites del mapa
    if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT)
        return true;

    // Verificar colisión con paredes
    return map[map_y][map_x] == 1;
}
