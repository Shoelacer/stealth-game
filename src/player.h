#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include "tilemap.h"

#define TILE_SIZE 32.0f
#define PLAYER_SPEED 400.0f

typedef struct
{
    float x, y;   // Top left of hitbox
    float vx, vy; // pixels/sec
} Player;

static inline void playerInit(Player *p, float x, float y)
{
    p->x = x;
    p->y = y;
    p->vx = 0;
    p->vy;
}

static inline void playerUpdate(Player *p, float dt)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    p->vx=0;
    p->vy=0;

    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    p->vy = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  p->vy =  PLAYER_SPEED;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  p->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) p->vx =  PLAYER_SPEED;

    //fix diag magnitude
    if(p->vx != 0 && p->vy !=0){
        p->vx *= 1.0f/SDL_sqrtf(2);
        p->vy *= 1.0f/SDL_sqrtf(2);
    }

    // move based on dt
    //p->x += p->vx * dt;
    //p->y += p->vy * dt;

    //bounds check, will replace with tile collision
    float newX = p->x + p->vx * dt;
    if (!rectHitsWall(newX, p->y, TILE, TILE))
        p->x = newX;

    float newY = p->y + p->vy * dt;
    if (!rectHitsWall(p->x, newY, TILE, TILE))
        p->y = newY;
}