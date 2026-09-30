#pragma once
#include "bullet.h"

typedef struct{
    float x,y;
    float w,h;
} Hitbox;


bool hittingWall(Hitbox *h){
    if(isWall((h->x)/TILE,(h->y)/TILE)) return true;
    if(isWall((h->x+h->w)/TILE,(h->y)/TILE)) return true;
    if(isWall((h->x)/TILE,(h->y+h->h)/TILE)) return true;
    if(isWall((h->x+h->w)/TILE,(h->y+h->h)/TILE)) return true;
    return false;
}