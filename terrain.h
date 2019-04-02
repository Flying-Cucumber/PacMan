#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#define WALL 0
#define PATH 1
#define SPAWN 2

struct slab{
    struct slab* up;
    struct slab* down;
    struct slab* left;
    struct slab* right;
    int type;
    int _x;
    int _y;
};

struct terrain{
    struct slab* initial_slab;
    struct slab* spawn_slab;
};

#endif