#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include <stdbool.h>
#include "terrain.h"

/* Directions of moving entities.
Entities are always TRYING to move
somewhere, so there is no "NULL" direction */
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 4

struct entity{
    struct slab* current_slab;
    int dir;
    bool is_alive;
};

struct pacman{
    struct entity* self;
    bool is_supersaiyen;
};

struct ghost{
    struct entity* slef;
};

#endif
