// src/main.c

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "tilemap.h"
#include "guard.h"
#include "bullet.h"

#define FIRE_RATE 1000000000/2

size_t current_bullet = 0;
bool running = true;

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

    Bullet bullets[10000];
    Guard guards[100];

    //INITIALIZE TEST GUARD
    guardInit(&guards[0],400,350,10,10);


    Uint64 lastTime=SDL_GetTicksNS();
    Uint64 lastBulletTime = SDL_GetTicksNS();

    while(running){

        // Time delay
        Uint64 now = SDL_GetTicksNS();
        float dt = (float)(now-lastTime)/1e9f;
        lastTime=now;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false; 
        }

        float mouse_x=0; float mouse_y=0;

        SDL_MouseButtonFlags Buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

        if (Buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)&& now-lastBulletTime>=FIRE_RATE) {

            float dx=mouse_x-(player.x+TILE_SIZE/2); float dy=mouse_y-(player.y+TILE_SIZE/2);
            if(SDL_fabs(dx)>SDL_fabs(dy)){
                dy/=SDL_fabs(dx/(TILE/2));
                dx/=SDL_fabs(dx/(TILE/2));
            }else{
                dx/=SDL_fabs(dy/(TILE/2));
                dy/=SDL_fabs(dy/(TILE/2));
            }


            bulletInit(
                &(bullets[current_bullet]),
                player.x+TILE_SIZE/2-BULLET_SIZE/2+dx*(1.00001+BULLET_SIZE/TILE),
                player.y+TILE_SIZE/2-BULLET_SIZE/2+dy*(1.00001+BULLET_SIZE/TILE),
                dx,
                dy, 
                (SDL_Color){0,0,255,255}, 
                3,
                true
            );



            current_bullet++;
            lastBulletTime = now;
        
        }

        playerUpdate(&player, bullets, dt);

        

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        renderTilemap(renderer);


        SDL_FRect playerRect = {player.x, player.y, 32.0f, 32.0f};
        SDL_SetRenderDrawColor(renderer, player.color.r, player.color.g, player.color.b, player.color.a);
        SDL_RenderFillRect(renderer, &playerRect);


        for(int i=0; i<current_bullet; i++){
            Bullet *b = &bullets[i];
            if(!b->active){
                Bullet temp = *b;
                *b = bullets[current_bullet-1];
                bullets[current_bullet-1].active=false;
                current_bullet--;
                continue;
            }
            bulletUpdate(b, dt);

            if(hittingLight(&(b->box))){
                SDL_FRect bulletRect = {b->box.x, b->box.y, b->box.w, b->box.h};
                SDL_SetRenderDrawColor(renderer, b->color.r, b->color.g, b->color.b, b->color.a);
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }

        for(int i=0; i<100; i++){
            Guard *g = &guards[i];
            if(!g->active){
                continue;
            }
            guardUpdate(g, bullets, &player, dt);
            if(hittingLight(&(g->box))){
                SDL_FRect guardRect = {g->box.x, g->box.y, g->box.w, g->box.h};
                SDL_SetRenderDrawColor(renderer, g->color.r, g->color.g, g->color.b, 255);
                SDL_RenderFillRect(renderer, &guardRect);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}