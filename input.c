
#include "raycaster.h"
#include <stdio.h>

void handle_input(Player *player, SDL_Event *event) {
    // Manejar eventos de teclado y ratón
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            exit(0);
        }
    }

    // Obtener estado del teclado
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    
    float new_x = player->x;
    float new_y = player->y;
    
    // Movimiento hacia adelante/atrás
    if (keyboard[SDL_SCANCODE_W]) {
        new_x += cosf(player->angle) * PLAYER_SPEED;
        new_y += sinf(player->angle) * PLAYER_SPEED;
    }
    if (keyboard[SDL_SCANCODE_S]) {
        new_x -= cosf(player->angle) * PLAYER_SPEED;
        new_y -= sinf(player->angle) * PLAYER_SPEED;
    }
    
    // Movimiento lateral (strafe) con A y D
    if (keyboard[SDL_SCANCODE_A]) {
        // Moverse a la izquierda (perpendicular a la dirección de visión)
        new_x -= sinf(player->angle) * PLAYER_SPEED;
        new_y += cosf(player->angle) * PLAYER_SPEED;
    }
    if (keyboard[SDL_SCANCODE_D]) {
        // Moverse a la derecha (perpendicular a la dirección de visión)
        new_x += sinf(player->angle) * PLAYER_SPEED;
        new_y -= cosf(player->angle) * PLAYER_SPEED;
    }
    
    // Rotación con flechas (mantenemos la funcionalidad)
    if (keyboard[SDL_SCANCODE_LEFT]) {
        player->angle -= ROTATION_SPEED;
    }
    if (keyboard[SDL_SCANCODE_RIGHT]) {
        player->angle += ROTATION_SPEED;
    }
    
    // Verificar colisiones antes de mover al jugador
    if (!check_collision(new_x, player->y)) {
        player->x = new_x;
    }
    if (!check_collision(player->x, new_y)) {
        player->y = new_y;
    }
}
