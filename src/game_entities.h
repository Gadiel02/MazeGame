#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

#include <SDL2/SDL.h>

// Constantes del mapa
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define MAX_RAINDROPS 1000

// Estructura para una gota de lluvia
typedef struct {
    float x;
    float y;
    float speed;
    int active;
} Raindrop;

// Estructura del jugador
typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    float angle;
    float fov;
    int show_minimap;
    int rain_enabled;
    Raindrop raindrops[MAX_RAINDROPS];
} Player;

// Funciones del jugador
void player_init(Player* player);
void player_update(Player* player);
void update_rain(Player* player);

// Funciones del mapa
int get_map_wall(int x, int y);

#endif
