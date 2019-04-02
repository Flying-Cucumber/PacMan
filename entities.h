#ifndef __ENTITIES_H__
#define __ENTITIES_H__
#include "terrain.h"
#include <stdbool.h>

struct pacman {
    struct slab* current_slab;
    bool is_alive;
    bool is_supersaiyen;
};

struct ghost {
    struct slab* current_slab;
    bool is_alive;
};

#endif
