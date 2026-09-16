#pragma once
#include <stdio.h>
#include <SDL3/SDL.h>

#define ORIGINAL_MAP { \
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1}, \
    {1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,1}, \
    {1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1}, \
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, \
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, \
}


#define MAP_W 25
#define MAP_H 19
#define TILE 32

enum Square {
    BLANK,
    WALL,
    VISIBLE
};

typedef struct
{
    int x,y;
} TileCoord;

static const int originalMap[MAP_H][MAP_W] = ORIGINAL_MAP;
int mapGrid[MAP_H][MAP_W] = ORIGINAL_MAP;

static inline bool isWall(int col, int row){
    if(col<0 || col >= MAP_W || row <0||row>=MAP_H) return true;
    return mapGrid[row][col]==1;
}



static inline bool rectHitsWall(float x, float y, float w, float h){
    int left = (int)(x/TILE);
    int right = (int)((x+w-.0001)/TILE);
    int top = (int)(y/TILE);
    int bottom = (int)((y+h-.0001)/TILE);

    for (int r = top; r <= bottom; r++)
        for (int c = left; c <= right; c++){
            if (isWall(c, r)){
                return true;
            }
        }
    return false;
}

static inline void renderTilemap(SDL_Renderer *renderer) {
    for (int r = 0; r < MAP_H; r++) {
        for (int c = 0; c < MAP_W; c++) {
            SDL_FRect rect = { c * TILE, r * TILE, TILE, TILE };

            switch(mapGrid[r][c]){
                case WALL:
                    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                break;
                case BLANK:
                    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                break;
                case VISIBLE:
                    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                break;
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}