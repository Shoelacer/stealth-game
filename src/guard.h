#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"
#include "hitbox.h"

extern size_t current_bullet;

typedef struct
{
    Hitbox box;
    float bulletSpeed;
    int health, damage;
    bool active;
    SDL_Color color;
    Uint64 lastBulletTime;
    Uint64 fireRate;
} Guard;


static inline void guardInit(Guard *g, int x, int y, int health, int damage){
    g->box.x=x;
    g->box.y=y;
    g->box.h=TILE;
    g->box.w=TILE;
    g->box.type = GUARD;
    g->health=health;
    g->damage=damage;
    g->active=true;
    g->color = (SDL_Color){255,255,0,255};
    g->lastBulletTime=SDL_GetTicks();
    g->fireRate = 1000/2;
}

static inline void guardUpdate(Guard *g, Bullet bullets[], Player *p, float dt){

    if(hittingBullet(&(g->box),bullets)){
        g->color.g=100;
        g->health--;
    }else{
        g->color.g=255;
    }

    if((SDL_GetTicks()-g->lastBulletTime)>=(g->fireRate)){
        bulletInit(&bullets[current_bullet], g->box.x, g->box.y, p->box.x-g->box.x,p->box.y-g->box.y,(SDL_Color){255,0,0,100},5, false);
        current_bullet++;
        g->lastBulletTime=SDL_GetTicks();
    }


    if(g->health<=0){
        g->active=false;
    }
}