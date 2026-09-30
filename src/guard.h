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
} Guard;


