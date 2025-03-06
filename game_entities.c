#include "game_entities.h"
#include "game.h"
#include <math.h>
#include <stdlib.h>

// Inicialización del jugador
void player_init(Player* player) {
    player->x = 2.0f;
    player->y = 2.0f;
    player->dx = 0;
    player->dy = 0;
    player->angle = 0.0f;
    player->fov = 60.0f;
    player->show_minimap = 1;
    player->rain_enabled = 0;

    // Inicializar gotas de lluvia con mejor distribución
    for (int i = 0; i < MAX_RAINDROPS; i++) {
        player->raindrops[i].x = (float)(rand() % WINDOW_WIDTH);
        player->raindrops[i].y = (float)(rand() % (WINDOW_HEIGHT * 2)) - WINDOW_HEIGHT;
        player->raindrops[i].speed = 8.0f + (rand() % 8); // Velocidades más variadas
        player->raindrops[i].active = 1;
    }
}

void update_rain(Player* player) {
    if (!player->rain_enabled) return;

    for (int i = 0; i < MAX_RAINDROPS; i++) {
        if (!player->raindrops[i].active) continue;

        player->raindrops[i].y += player->raindrops[i].speed;

        // Si la gota llega al suelo, reiniciarla arriba con posición aleatoria
        if (player->raindrops[i].y > WINDOW_HEIGHT) {
            player->raindrops[i].y = -10.0f - (rand() % 20); // Más variación en la altura inicial
            player->raindrops[i].x = (float)(rand() % WINDOW_WIDTH);
            player->raindrops[i].speed = 8.0f + (rand() % 8);
        }
    }
}

void player_update(Player* player) {
    check_collision(player);
    update_rain(player);
}

// Definición del mapa
static const int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,1,0,0,1,1,1,0,1,0,0,1,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1},
    {1,0,0,1,1,1,1,0,0,0,0,1,0,1,0,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1},
    {1,1,1,1,0,0,1,0,0,1,0,0,0,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1},
    {1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int get_map_wall(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return 1; // Out of bounds is treated as wall
    }
    return map[y][x];
}
