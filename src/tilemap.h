#pragma once
#include <stdio.h>
#include <SDL3/SDL.h>

#define MAP_W 25
#define MAP_H 19
#define TILE 32

static const int mapGrid[MAP_H][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

static inline bool isWall(int col, int row){
    if(col<0 || col >= MAP_W || row <0||row>=MAP_H) return true;
    return mapGrid[row][col]==1;
}

//TODO:
//Make it so that it finds the edge of the current tile and moves into it basically

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
            if (mapGrid[r][c] == 1) {
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}