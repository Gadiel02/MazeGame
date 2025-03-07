#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "game_entities.h"

// Estructuras
typedef struct {
    float distance;
    int wall_type;
    int orientation;
} RaycastHit;

// Prototipos de funciones
void handle_player_input(SDL_Event* event, Player* player);
void render_frame(SDL_Renderer* renderer, Player* player);
RaycastHit cast_ray(float ray_angle, const Player* player);
void check_collision(Player* player);
void draw_minimap(SDL_Renderer* renderer, const Player* player);

// Constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MINIMAP_SIZE 140
#define MINIMAP_X 20
#define MINIMAP_Y 20

#endif
