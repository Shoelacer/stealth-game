// src/main.c

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "tilemap.h"
#include "guard.h"
#include "bullet.h"

#define FIRE_RATE 1000/2
#define SUPER_SIGHT_DURATION 2000
#define SUPER_SPEED_DURATION 4000

size_t current_bullet = 0;
bool running = true;

static void readMapFromText(int *grid, Guard guards[]);
void initGame(SDL_Renderer *renderer);
void startMenu(SDL_Renderer *renderer);
void initMapMaker(SDL_Renderer *renderer);


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

    startMenu(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void startMenu(SDL_Renderer *renderer){
    while(true){


        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return;
        }

        const bool *keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_1]){
            initGame(renderer);
            break;
        }else if(keys[SDL_SCANCODE_2]){
            initMapMaker(renderer);
        }
        
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void initMapMaker(SDL_Renderer *renderer){
    while(true){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return;
        }

        const bool *keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_ESCAPE]){
            startMenu(renderer);
            break;
        }else if(keys[SDL_SCANCODE_1]){
            //break;
        }else if(keys[SDL_SCANCODE_2]){
        }
        
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void initGame(SDL_Renderer *renderer){

    //Game stuff
    Player player;
    playerInit(&player, 320.0f, 320.0f);

    Bullet bullets[10000];
    Guard guards[100];
    int guardsAlive = 0;

    readMapFromText((int*)originalMap, guards);
    readMapFromText((int*)mapGrid, guards);
    memcpy(visibleGrid, originalVisibleGrid, sizeof(visibleGrid));   
    Uint64 lastTime=SDL_GetTicks();
    Uint64 lastBulletTime = lastTime;
    Uint64 powerupTime = -1e9;

    bool powerupsActive[2] = {false, false};
    int powerupSelected = 1;

    while(running){

        // Time delay
        Uint64 now = SDL_GetTicks();
        float dt = (float)(now-lastTime)/1e3f;
        lastTime=now;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false; 
        }

        float mouse_x=0; float mouse_y=0;

        SDL_MouseButtonFlags Buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        const bool *keys = SDL_GetKeyboardState(NULL);
        
        if(keys[SDL_SCANCODE_ESCAPE]){
            startMenu(renderer);
            break;
        }
        if(keys[SDL_SCANCODE_1]) powerupSelected = 1;
        else if(keys[SDL_SCANCODE_2]) powerupSelected = 2;
        else if(keys[SDL_SCANCODE_3]) powerupSelected = 3;

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

        if (powerupSelected == 1 &&Buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)&& now-powerupTime>=SUPER_SIGHT_DURATION*2){
            player.sight_range *=2;
            powerupsActive[0] = true;
            powerupTime=now;
        }

        if ((now - powerupTime >= SUPER_SIGHT_DURATION) && powerupsActive[0]) {
            player.sight_range /= 2;  
            powerupsActive[0]=false;
        }


        if (powerupSelected == 2 && Buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)&& now-powerupTime>=SUPER_SPEED_DURATION*2){
            player.speed *=2;
            powerupsActive[1] = true;
            powerupTime=now;
        }

        if ((now - powerupTime >= SUPER_SPEED_DURATION) && powerupsActive[1]) {
            player.speed /= 2;  
            powerupsActive[1]=false;
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
            }
            bulletUpdate(b, dt);

            if(hittingLight(&(b->box))){
                SDL_FRect bulletRect = {b->box.x, b->box.y, b->box.w, b->box.h};
                SDL_SetRenderDrawColor(renderer, b->color.r, b->color.g, b->color.b, b->color.a);
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }
        
        guardsAlive=0;
        for(int i=0; i<100; i++){
            Guard *g = &guards[i];
            if(!g->active){
                continue;
            }
            guardsAlive++;
            guardUpdate(g, bullets, &player, dt);
            if(hittingLight(&(g->box))){
                SDL_FRect guardRect = {g->box.x, g->box.y, g->box.w, g->box.h};
                SDL_SetRenderDrawColor(renderer, g->color.r, g->color.g, g->color.b, 255);
                SDL_RenderFillRect(renderer, &guardRect);
            }
        }
        
        if(guardsAlive==0){
            printf("YOU WON!\n");
            fflush(stdout);
            running=false;
        }


        SDL_RenderPresent(renderer);
        SDL_Delay(16);

    }
}


static void readMapFromText(int *grid, Guard guards[]){
    FILE *fp = fopen("map.txt","r");
    if (fp == NULL) {
        printf("Error opening file\n");
        fflush(stdout);
        return;
    }
    printf("READING\n");
    fflush(stdout);
    char line[MAP_W+2];
    int row = 0;
    int guard = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line[0] == 'G') {
            int x=0;
            int y=0;
            printf("%d, %d\n",x,y);
            fflush(stdout);
            if (sscanf(line, "G %d %d", &x, &y) == 2) {
                printf("%d, %d\n",x,y);
                guardInit(&guards[guard], x, y, 10, 10);
                guard++;
            }else{
                printf("%d, %d bruh\n",x,y);
                fflush(stdout);
            }
        }else{
            for(int i=0; i<MAP_W; i++){
                if(line[i]=='0') *(grid + (row*MAP_W+i)) = 0;
                else *(grid+(row*MAP_W+i))=1;
            }
            row++;
        }
    }

    fclose(fp);
}
