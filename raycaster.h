#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 5
#define TILE_SIZE 64
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FOV (M_PI / 3)
#define PLAYER_SPEED 5.0
#define ROTATION_SPEED 0.1

extern int map[MAP_HEIGHT][MAP_WIDTH];

typedef struct {
    float x, y, angle;
} Player;

void load_map(const char *filename);
bool check_collision(float x, float y);
void render_walls(SDL_Renderer *renderer, Player *player);
void handle_input(Player *player, SDL_Event *event);

#endif // RAYCASTER_H
