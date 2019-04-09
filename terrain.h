#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#define WALL 0
#define PATH 1
#define SPAWN 2
#define SLAB_SIZE 32

struct slab{
    struct slab* up;
    struct slab* down;
    struct slab* left;
    struct slab* right;
    int type;
    int x;
    int y;
};

struct terrain{
    struct slab* initial_slab;
    struct slab* spawn_slab;
    struct slab* ghost_house;
};

#endif