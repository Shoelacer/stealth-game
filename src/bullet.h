#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"

typedef struct{
    float x, y;
    float vx, vy;
    float damage;
} Bullet;

static inline void bulletInit(Bullet *b, float x, float y, float vx, float vy)
{
    b->x = x;
    b->y = y;
    b->vx = vx;
    b->vy = vy;
}

static inline void bulletUpdate(Bullet *b, float dt){

}