#include "game.h"
#include "game_entities.h"
#include <math.h>

#define PI 3.14159265359f
#define TWO_PI (2.0f * PI)
#define MOVE_SPEED 0.1f
#define TURN_SPEED 0.05f
#define BORDER_SIZE 3

// Draw rain
static void draw_rain(SDL_Renderer* renderer, const Player* player) {
    if (!player->rain_enabled) return;

    SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
    for (int i = 0; i < MAX_RAINDROPS; i++) {
        if (!player->raindrops[i].active) continue;

        int x = (int)player->raindrops[i].x;
        int y = (int)player->raindrops[i].y;

        for (int dx = 0; dx < 2; dx++) {
            SDL_RenderDrawLine(renderer, 
                             x + dx, y, 
                             x + dx, y + 5);
        }
    }
}

// Raycasting
RaycastHit cast_ray(float ray_angle, const Player* player) {
    RaycastHit hit;
    float sin_a = sinf(ray_angle);
    float cos_a = cosf(ray_angle);
    float ray_x = player->x;
    float ray_y = player->y;
    float distance = 0.0f;

    float step_size = 0.005f;
    while (distance < 20.0f) {
        ray_x += cos_a * step_size;
        ray_y += sin_a * step_size;
        distance += step_size;

        if (get_map_wall((int)ray_x, (int)ray_y)) {
            hit.wall_type = 1;
            float wall_x = ray_x - floor(ray_x);
            hit.orientation = (wall_x < 0.01f || wall_x > 0.99f) ? 1 : 0;
            break;
        }
    }

    float angle_diff = ray_angle - player->angle;
    while (angle_diff < -PI) angle_diff += TWO_PI;
    while (angle_diff > PI) angle_diff -= TWO_PI;
    hit.distance = distance * cosf(angle_diff);

    return hit;
}

// Draw wall stripe
static void draw_wall_stripe(SDL_Renderer* renderer, int x, const RaycastHit* hit) {
    float height_scale = 1.5f;
    int h = (int)(WINDOW_HEIGHT * height_scale / (hit->distance + 0.01f));
    int draw_start = (WINDOW_HEIGHT - h) / 2;
    int draw_end = draw_start + h;

    if (draw_start < 0) draw_start = 0;
    if (draw_end >= WINDOW_HEIGHT) draw_end = WINDOW_HEIGHT - 1;

    // Render solid color walls
    Uint8 r, g, b;
    if (hit->orientation == 0) {
        r = 255; g = 100; b = 100;
    } else {
        r = 180; g = 70; b = 70;
    }
    float darkness = 1.0f / (1.0f + hit->distance * 0.2f);
    SDL_SetRenderDrawColor(renderer, r * darkness, g * darkness, b * darkness, 255);
    SDL_RenderDrawLine(renderer, x, draw_start, x, draw_end);
}

// Draw minimap
void draw_minimap(SDL_Renderer* renderer, const Player* player) {
    if (!player->show_minimap) return;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect border = {
        MINIMAP_X - BORDER_SIZE,
        MINIMAP_Y - BORDER_SIZE,
        MINIMAP_SIZE + (BORDER_SIZE * 2),
        MINIMAP_SIZE + (BORDER_SIZE * 2)
    };
    SDL_RenderFillRect(renderer, &border);

    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_Rect minimap_bg = {MINIMAP_X, MINIMAP_Y, MINIMAP_SIZE, MINIMAP_SIZE};
    SDL_RenderFillRect(renderer, &minimap_bg);

    float tile_width = (float)MINIMAP_SIZE / MAP_WIDTH;
    float tile_height = (float)MINIMAP_SIZE / MAP_HEIGHT;

    // Draw walls
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (get_map_wall(x, y)) {
                SDL_Rect wall = {
                    MINIMAP_X + (int)(x * tile_width),
                    MINIMAP_Y + (int)(y * tile_height),
                    (int)tile_width + 1,
                    (int)tile_height + 1
                };
                SDL_RenderFillRect(renderer, &wall);
            }
        }
    }

    // Draw player
    int player_x = MINIMAP_X + (int)(player->x * tile_width);
    int player_y = MINIMAP_Y + (int)(player->y * tile_height);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect player_dot = {player_x - 2, player_y - 2, 4, 4};
    SDL_RenderFillRect(renderer, &player_dot);

    // Draw player direction
    int dir_x = player_x + (int)(cosf(player->angle) * 15);
    int dir_y = player_y + (int)(sinf(player->angle) * 15);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(renderer, player_x, player_y, dir_x, dir_y);
}

// Render frame
void render_frame(SDL_Renderer* renderer, Player* player) {
    // Draw ceiling
    SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
    SDL_Rect ceiling = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/2};
    SDL_RenderFillRect(renderer, &ceiling);

    // Draw floor
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Rect floor = {0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT/2};
    SDL_RenderFillRect(renderer, &floor);

    // Raycast and draw walls
    float ray_angle = player->angle - (player->fov * PI / 180.0f) / 2.0f;
    float angle_step = (player->fov * PI / 180.0f) / WINDOW_WIDTH;

    for (int x = 0; x < WINDOW_WIDTH; x++) {
        RaycastHit hit = cast_ray(ray_angle, player);
        draw_wall_stripe(renderer, x, &hit);
        ray_angle += angle_step;
    }

    // Draw rain
    draw_rain(renderer, player);

    // Draw minimap
    draw_minimap(renderer, player);
}

void handle_player_input(SDL_Event* event, Player* player) {
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    // Toggle minimap with 'M' key
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_m) {
            player->show_minimap = !player->show_minimap;
        }
        // Toggle rain with 'R' key
        else if (event->key.keysym.sym == SDLK_r) {
            player->rain_enabled = !player->rain_enabled;
        }
    }

    // Rotation
    if (keyboard[SDL_SCANCODE_LEFT]) {
        player->angle -= TURN_SPEED;
        if (player->angle < 0) player->angle += TWO_PI;
    }
    if (keyboard[SDL_SCANCODE_RIGHT]) {
        player->angle += TURN_SPEED;
        if (player->angle > TWO_PI) player->angle -= TWO_PI;
    }

    // Movement
    player->dx = 0;
    player->dy = 0;
    if (keyboard[SDL_SCANCODE_W]) {
        player->dx += cosf(player->angle) * MOVE_SPEED;
        player->dy += sinf(player->angle) * MOVE_SPEED;
    }
    if (keyboard[SDL_SCANCODE_S]) {
        player->dx -= cosf(player->angle) * MOVE_SPEED;
        player->dy -= sinf(player->angle) * MOVE_SPEED;
    }
    if (keyboard[SDL_SCANCODE_A]) {
        player->dx += cosf(player->angle - PI/2) * MOVE_SPEED;
        player->dy += sinf(player->angle - PI/2) * MOVE_SPEED;
    }
    if (keyboard[SDL_SCANCODE_D]) {
        player->dx += cosf(player->angle + PI/2) * MOVE_SPEED;
        player->dy += sinf(player->angle + PI/2) * MOVE_SPEED;
    }

    check_collision(player);
}

void check_collision(Player* player) {
    float new_x = player->x + player->dx;
    float new_y = player->y + player->dy;

    if (!get_map_wall((int)new_x, (int)player->y)) {
        player->x = new_x;
    }
    if (!get_map_wall((int)player->x, (int)new_y)) {
        player->y = new_y;
    }
}

void init_game(SDL_Renderer* renderer) {
    //No texture loading needed
}
