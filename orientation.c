#include "raycaster.h"
#include <math.h>

// Map definition
int map[MAP_HEIGHT][MAP_WIDTH];

void render_walls(SDL_Renderer *renderer, Player *player) {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float ray_angle = player->angle - (FOV / 2) + ((float)i / SCREEN_WIDTH) * FOV;
        float ray_x = player->x;
        float ray_y = player->y;
        float ray_dx = cosf(ray_angle);
        float ray_dy = sinf(ray_angle);
        
        // Normalizar dirección
        float length = sqrtf(ray_dx * ray_dx + ray_dy * ray_dy);
        ray_dx /= length;
        ray_dy /= length;;

        float distance = 0;
        bool hit_wall = false;
        
        while (!hit_wall) {
            int map_x = (int)(ray_x / TILE_SIZE);
            int map_y = (int)(ray_y / TILE_SIZE);
            
            if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT) {
                hit_wall = true;
                distance = SCREEN_WIDTH; // Valor grande para evitar división por cero
            } else if (map[map_y][map_x] == 1) {
                hit_wall = true;
                
                // Calcular distancia exacta
                distance = sqrtf((ray_x - player->x) * (ray_x - player->x) + 
                               (ray_y - player->y) * (ray_y - player->y));
                
                // Corregir efecto ojo de pez
                distance *= cosf(player->angle - ray_angle);
            } else {
                ray_x += ray_dx * 2.0;
                ray_y += ray_dy * 2.0;
            }
        }
        
        // Calcular altura de la pared
        int wall_height = (int)(SCREEN_HEIGHT / distance * TILE_SIZE);
        if (wall_height > SCREEN_HEIGHT) wall_height = SCREEN_HEIGHT;
        
        int wall_top = (SCREEN_HEIGHT / 2) - (wall_height / 2);
        int wall_bottom = wall_top + wall_height;
        
        // Determinar el color según la orientación
        // Paredes Norte-Sur serán rojas, Este-Oeste serán azules
        if (fabs(ray_dy) > fabs(ray_dx)) {
            // Norte/Sur
            SDL_SetRenderDrawColor(renderer, 200, 80, 80, 255); // Rojo
        } else {
            // Este/Oeste
            SDL_SetRenderDrawColor(renderer, 80, 80, 200, 255); // Azul
        }
        SDL_RenderDrawLine(renderer, i, wall_top, i, wall_bottom);
    }
}
