#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"

#define BULLET_SPEED 10

typedef struct{
    float x, y;
    float vx, vy;
    float damage;
} Bullet;

static inline void bulletInit(Bullet *b, float x, float y, float vx, float vy)
{
    b->x = x;
    b->y = y;
    b->vx = BULLET_SPEED*(vx/sqrtf(vx*vx+vy*vy));
    b->vy = BULLET_SPEED*(vy/sqrtf(vx*vx+vy*vy));
}

static inline void bulletUpdate(Bullet *b, float dt){
    b->x+=b->vx;
    b->y+=b->vy;
}

