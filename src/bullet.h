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
    int bounces;
    int max_bounces;
    bool friendly;
    SDL_Color color;
} Bullet;


static inline void bulletInit(Bullet *b, float x, float y, float vx, float vy, SDL_Color color, int max_bounces, bool friendly)
{
    b->x = x;
    b->y = y;
    b->vx = BULLET_SPEED*(vx/sqrtf(vx*vx+vy*vy));
    b->vy = BULLET_SPEED*(vy/sqrtf(vx*vx+vy*vy));
    b->active=true;
    b->color=color;
    b->max_bounces=max_bounces;
    b->w=BULLET_SIZE;
    b->h=BULLET_SIZE;
    b->box.x=x;
    b->box.y=y;
    b->box.w=BULLET_SIZE;
    b->box.h=BULLET_SIZE;
    b->box.type = BULLET;
    b->bounces=0;
    b->friendly=friendly;
    
}

static inline void bulletUpdate(Bullet *b, float dt){
    Hitbox original = b->box;
    b->box.x+=b->vx;
    if(hittingWall(&(b->box))){
        if(b->bounces<b->max_bounces){
            b->bounces++;
            b->vx*=-1;
        }
        else
            b->active=false;
    }
    b->box.x=original.x;
    b->box.y+=b->vy;
    if(hittingWall(&(b->box))){
        if(b->bounces<b->max_bounces){
            b->bounces++;
            b->vy*=-1;
        }
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
        if((h->type==GUARD)&&(b->friendly==false))continue;
        if((h->type==PLAYER)&&(b->friendly==true))continue;
        SDL_Rect rb = {b->box.x, b->box.y, b->box.w, b->box.h};
        SDL_Rect rp = {h->x, h->y, TILE, TILE};
        if(SDL_HasRectIntersection(&rb, &rp)){
            b->active=false;
            return true;
        }
    }
    return false;
}