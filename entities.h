#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include <stdbool.h>
#include "terrain.h"

struct entity{
    struct slab* current_slab;
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
