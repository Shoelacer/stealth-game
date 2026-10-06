#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"
#include "bullet.h"
#include "hitbox.h"

#define TILE_SIZE 32.0f
#define PLAYER_SPEED 200.0f
#define FOV_ANGLE 3.14159*2 * (120)/360

extern bool running;

typedef struct
{
    Hitbox box;
    float x, y;   // Top left of hitbox
    float vx, vy; // pixels/sec
    float health;
    SDL_Color color;

} Player;

static inline void playerInit(Player *p, float x, float y)
{
    p->x = x;
    p->y = y;
    p->vx = 0;
    p->vy = 0;
    p->box.type = PLAYER;
    p->color = (SDL_Color){255,255,255,255};
    p->health=10;
}

// RAYTRACING ALGORITHM
// credits: https://aaaa.sh/creatures/dda-algorithm-interactive/ 
// learned the algorithm from the site above
static inline void drawLine(float x1, float y1, float x2, float y2){
    if(x2==x1)return;


    float dx = x2-x1;
    float dy = y2-y1;
    float slope = (dy)/(dx);
    float angle = atan2f(-dy,dx);

    for(float theta = angle - FOV_ANGLE/2; theta<=angle+FOV_ANGLE/2; theta+=.1){

        TileCoord white = {(int)x1,(int)y1};
        float xDist = cosf(theta);
        float yDist = sinf(theta);


        float distBetweenRows = sqrt(1+ (xDist/yDist)*(xDist/yDist));
        float distBetweenColumns = sqrt(1+ (yDist/xDist)*(yDist/xDist));

        float distToY = distBetweenRows;
        float distToX = distBetweenColumns;
        

        for(int i=0; i<10; i++){
            if(white.x<0||white.x>=MAP_W||white.y<0||white.y>=MAP_H)break;
            if(mapGrid[white.y][white.x]==WALL) break;
            mapGrid[white.y][white.x]=2;
            visibleGrid[white.y][white.x]=1;
            if(distToY>distToX){
                
                    white.x+=xDist/SDL_fabs(xDist);
                    distToX+=distBetweenColumns;
            }else{
                    white.y+=-yDist/SDL_fabs(yDist);
                    distToY+=distBetweenRows;
            }
        }
    }
}

static inline void playerUpdate(Player *p, Bullet bullets[], float dt)
{
    const bool *keys = SDL_GetKeyboardState(NULL);

    float mouseX = 0, mouseY = 0;

    SDL_GetMouseState(&mouseX,&mouseY);

    p->vx=0;
    p->vy=0;

    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    p->vy = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  p->vy =  PLAYER_SPEED;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  p->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) p->vx =  PLAYER_SPEED;

    //fix diag magnitude
    if(p->vx != 0 && p->vy !=0){
        p->vx *= 1.0f/SDL_sqrtf(2);
        p->vy *= 1.0f/SDL_sqrtf(2);
    }

    // snap coords?
    float newX = p->x + p->vx * dt;
    if (rectHitsWall(newX, p->y, TILE-1, TILE-1)&&p->vx!=0) {
        if (p->vx < 0) {
            // Moving left
            int wallCol = (int)(newX / TILE);
            p->x = (wallCol + 1) * TILE;
        } else {
            //moving right
            int wallCol = (int)((newX + TILE) / TILE);
            p->x = wallCol * TILE - TILE;
        }
    } else {
        p->x = newX;
    }

    float newY = p->y + p->vy * dt;
    if (rectHitsWall(p->x, newY, TILE-1, TILE-1)&&p->vy!=0) {
        if (p->vy < 0) {
            int wallRow = (int)(newY / TILE);
            p->y = (wallRow + 1) * TILE;
        } else {
            int wallRow = (int)((newY + TILE) / TILE);
            p->y = wallRow * TILE - TILE;
        }
    } else {
        p->y = newY;
    }   

    p->box.x=p->x;
    p->box.y=p->y;

    memcpy(mapGrid, originalMap, sizeof(mapGrid));

    if(hittingBullet(&(p->box),bullets)){
        p->color.b=100;
        p->color.g=100;
        p->health--;
    }else{
        p->color.b=255;
        p->color.g=255;
    }

    //mapGrid[(int)((mouseY)/TILE)][(int)((mouseX)/TILE)] = 2;

    if(p->health<=0){
        running=false;
    }

    drawLine(p->x/TILE+0.5f, p->y/TILE+0.5f, mouseX/TILE, mouseY/TILE);



}

