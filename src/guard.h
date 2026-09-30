#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"
#include "hitbox.h"

typedef struct
{
    Hitbox box;
    float bulletSpeed;
    int health, damage;
    bool active;
} Guard;


static inline void guardInit(Guard *g, int x, int y, int health, int damage){
    g->box.x=x;
    g->box.y=y;
    g->box.h=TILE;
    g->box.w=TILE;
    g->health=health;
    g->damage=damage;
    g->active=true;
}

static inline void guardUpdate(Guard *g, float dt){

}