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
    float start_x, start_y;
    bool active;
    SDL_Color color;
    Uint64 lastBulletTime;
    Uint64 fireRate;
    float last_seen_x, last_seen_y;
    Uint64 start_wait;
    bool waiting;
} Guard;


static inline void guardInit(Guard *g, int x, int y, int health, int damage){
    g->box.x=x;
    g->box.y=y;
    g->start_x= x;
    g->start_y=y;
    g->last_seen_x= x;
    g->last_seen_y=y;
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

    if(g->health<=0){
        g->active=false;
    }

    bool seesPlayer = true;
    double px = p->x+TILE/2;
    double py = p->y+TILE/2;
    double gx = g->box.x+TILE/2;
    double gy = g->box.y+TILE/2;

    double dx = (px-gx);
    double dy = (py-gy);
    double dist = sqrtf(dx*dx+dy*dy);
    dx*=1/dist;
    dy*=1/dist;
    
    Hitbox path = {gx,gy,4,4};

    while(fabs(path.x-px)>.001 && fabs(path.y-py)>.001){
        path.x+=dx;
        path.y+=dy;

        if(hittingWall(&path)) {
            seesPlayer = false;
            break;
        }

        if(((path.x+dx)-px)/fabs(((path.x+dx)-px))!=(path.x-px)/fabs(path.x-px)) break;
        if(((path.y+dy)-py)/fabs(((path.y+dy)-py))!=(path.y-py)/fabs(path.y-py)) break;
        seesPlayer = true;
    }

    if(seesPlayer){
        g->box.x+=dx;
        g->box.y+=dy;

        g->last_seen_x = px;
        g->last_seen_y = py;
        

        if((SDL_GetTicks()-g->lastBulletTime)>=(g->fireRate)){
            bulletInit(&bullets[current_bullet], g->box.x, g->box.y, p->box.x-g->box.x,p->box.y-g->box.y,(SDL_Color){255,0,0,100},5, false);
            current_bullet++;
            g->lastBulletTime=SDL_GetTicks();
        }
    }else{
        double dx = (g->last_seen_x-gx);
        double dy = (g->last_seen_y-gy);
        double dist = sqrtf(dx*dx+dy*dy);
        dx*=1/dist;
        dy*=1/dist;
        g->box.x+=dx;
        g->box.y+=dy;

        if(fabs(g->last_seen_x-gx)<2 && fabs(g->last_seen_y-gy)<2 && !g->waiting){
            g->start_wait = SDL_GetTicks();
            g->waiting=true;
        }
        if(SDL_GetTicks()-g->start_wait>1000 && g->waiting){
            g->box.x=g->start_x;
            g->box.y=g->start_y;
            g->waiting=false;
            g -> last_seen_x = (g->start_x)+15;
            g -> last_seen_y = (g->start_y)+15;
        }
    }

}