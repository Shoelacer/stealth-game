#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "tilemap.h"

typedef struct
{
    float x, y;   // Top left of hitbox
    float bulletSpeed;
    int health, damage;
} Guard;


