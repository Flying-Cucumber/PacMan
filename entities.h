#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include <stdbool.h>
#include "terrain.h"
            
/* Entities are always TRYING to move
* somewhere, so there is no "NULL" direction. */

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define NORMAL 0
#define SCARED 1
#define DEAD 2

/* The basic object is "entity". It is shared
* by both Pac-Man and the ghosts */
struct entity{
    int speed;
    struct slab* current_slab;
    int dir;
    bool is_alive;
};


/* Pac-Man has additionally a supersaiyen
* mode: when he can eat the ghosts. */
struct pacman{
    struct entity* self;
};

/* The ghosts are always trying to reach a
* specific slab : their "target" attribute */
struct ghost{
    struct entity* self;
    struct slab* target;
    int state;
};

struct fruit{
    struct entity* self;
    int points;
};

#endif
