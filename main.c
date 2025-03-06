
#include <stdio.h>
#include <SDL2/SDL.h>
#include "raycaster.h"

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window *window = SDL_CreateWindow(
        "3D Maze Raycaster",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    Player player = {
        .x = TILE_SIZE * 1.5,  // Posición inicial
        .y = TILE_SIZE * 1.5,
        .angle = 0  // Mirando al este
    };

    printf("Loading map...\n");
    load_map("map.txt");
    printf("Map loaded. Starting game loop.\n");

    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    
    while (1) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Manejar entrada
        handle_input(&player, &event);
        
        // Limpiar pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Negro
        SDL_RenderClear(renderer);
        
        // Dibujar cielo y suelo
        SDL_Rect sky = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
        SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255); // Azul cielo
        SDL_RenderFillRect(renderer, &sky);
        
        SDL_Rect floor = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Gris oscuro
        SDL_RenderFillRect(renderer, &floor);
        
        // Renderizar paredes
        render_walls(renderer, &player);
        
        // Mostrar todo en pantalla
        SDL_RenderPresent(renderer);
        
        // Limitar FPS
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
