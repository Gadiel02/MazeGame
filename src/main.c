#include <SDL2/SDL.h>
#include "game.h"
#include "game_entities.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Verificar variable de entorno DISPLAY
    char* display = getenv("DISPLAY");
    if (!display) {
        fprintf(stderr, "Error: Variable DISPLAY no está configurada.\n");
        return 1;
    }
    printf("Variable DISPLAY configurada como: %s\n", display);

    // Forzar renderizado por software
    putenv("SDL_RENDER_DRIVER=software");

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    printf("SDL inicializado correctamente.\n");

    // Crear ventana
    SDL_Window* window = SDL_CreateWindow("Raycaster Demo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    printf("Ventana creada correctamente.\n");

    // Crear renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    printf("Renderer creado correctamente.\n");

    // Inicializar jugador
    Player player;
    player_init(&player);
    printf("Jugador inicializado.\n");

    SDL_Event event;
    int running = 1;
    int frame_count = 0;

    printf("Iniciando bucle principal del juego...\n");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            handle_player_input(&event, &player);
        }

        player_update(&player);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_frame(renderer, &player);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Aproximadamente 60 FPS

        frame_count++;
        if (frame_count % 60 == 0) {
            printf("Frames renderizados: %d\n", frame_count);
        }
    }

    printf("Limpiando recursos...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Juego finalizado correctamente.\n");

    return 0;  // <-- Asegura que main termina correctamente
}
