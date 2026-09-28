#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"

#define BULLET_SPEED 10
#define BULLET_SIZE 4.0f

typedef struct{
    float x, y;
    float vx, vy;
    float w,h;
    float damage;
    bool active;
    bool is_bouncy;
    SDL_Color color;
} Bullet;


static inline void bulletInit(Bullet *b, float x, float y, float vx, float vy, SDL_Color color, bool is_bouncy)
{
    b->x = x;
    b->y = y;
    b->vx = BULLET_SPEED*(vx/sqrtf(vx*vx+vy*vy));
    b->vy = BULLET_SPEED*(vy/sqrtf(vx*vx+vy*vy));
    b->active=true;
    b->color=color;
    b->is_bouncy=is_bouncy;
    b->w=BULLET_SIZE;
    b->h=BULLET_SIZE;
}

static inline void bulletUpdate(Bullet *b, float dt){
    if(isWall((b->x+b->vx)/TILE,(b->y)/TILE)){
        if(b->is_bouncy)
            b->vx*=-1;
        else
            b->active=false;
    }
    if(isWall((b->x)/TILE,(b->y+b->vy)/TILE)){
        if(b->is_bouncy)
            b->vy*=-1;
        else
            b->active=false;
    }
    b->x+=b->vx;
    b->y+=b->vy;
}

