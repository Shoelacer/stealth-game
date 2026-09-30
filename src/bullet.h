#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"
#include "hitbox.h"

#define BULLET_SPEED 6
#define BULLET_SIZE 4.0f

typedef struct{
    Hitbox box;
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
    b->box.x=x;
    b->box.y=y;
    b->box.w=BULLET_SIZE;
    b->box.h=BULLET_SIZE;
    
}

static inline void bulletUpdate(Bullet *b, float dt){
    Hitbox original = b->box;
    b->box.x+=b->vx;
    if(hittingWall(&(b->box))){
        if(b->is_bouncy)
            b->vx*=-1;
        else
            b->active=false;
    }
    b->box.x=original.x;
    b->box.y+=b->vy;
    if(hittingWall(&(b->box))){
        if(b->is_bouncy)
            b->vy*=-1;
        else
            b->active=false;
    }
    b->box.y=original.y;
    b->box.x+=b->vx;
    b->box.y+=b->vy;
    if(hittingWall(&(b->box)))b->active=false;
}


bool hittingBullet(Hitbox *h, Bullet bullets[]){
    for(int i=0; i<10000; i++){
        
        Bullet *b = &bullets[i];
        if(b->active==false)continue;
        SDL_Rect rb = {b->box.x, b->box.y, b->box.w, b->box.h};
        SDL_Rect rp = {h->x, h->y, TILE, TILE};
        if(SDL_HasRectIntersection(&rb, &rp)){
            return true;
        }
    }
    return false;
}