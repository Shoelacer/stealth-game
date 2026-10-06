#pragma once
#include "bullet.h"

typedef enum Type {
    PLAYER,
    GUARD,
    BULLET
} Type;

typedef struct{
    float x,y;
    float w,h;
    Type type;
} Hitbox;


bool hittingWall(Hitbox *h){
    if(isWall((h->x)/TILE,(h->y)/TILE)) return true;
    if(isWall((h->x+h->w)/TILE,(h->y)/TILE)) return true;
    if(isWall((h->x)/TILE,(h->y+h->h)/TILE)) return true;
    if(isWall((h->x+h->w)/TILE,(h->y+h->h)/TILE)) return true;
    return false;
}

bool hittingLight(Hitbox *h){
    if(isLit((h->x)/TILE,(h->y)/TILE)) return true;
    if(isLit((h->x+h->w)/TILE,(h->y)/TILE)) return true;
    if(isLit((h->x)/TILE,(h->y+h->h)/TILE)) return true;
    if(isLit((h->x+h->w)/TILE,(h->y+h->h)/TILE)) return true;
    return false;
}