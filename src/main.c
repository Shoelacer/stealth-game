// src/main.c

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "tilemap.h"

int main(int argc, char *argv[]){
    (void)argc; (void)argv; // to avoid compiler warnings about unused vars

    // Try initializing the video if doesn't work then log error
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    // Try initializing window/renderer
    if(!SDL_CreateWindowAndRenderer("Stealth Game", 800, 600, 0, &window, &renderer)){
        SDL_Log("Window or Renderer failed: %s", SDL_GetError());
        SDL_Quit(); //Since we already initialized SDL we have to quit it
        return 1;
    }

    //Game stuff
    Player player;
    playerInit(&player, 320.0f, 320.0f);

    bool running = true;

    Uint64 lastTime=SDL_GetTicksNS();

    while(running){

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false; //Quits app
        }

        // Time delay
        Uint64 now = SDL_GetTicksNS();
        float dt = (float)(now-lastTime)/1e9f;
        lastTime=now;

        playerUpdate(&player, dt);

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        renderTilemap(renderer);


        SDL_FRect playerRect = {player.x, player.y, 32.0f, 32.0f};
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}