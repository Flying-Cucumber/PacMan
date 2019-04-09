#ifndef __TERRAIN_MANIPULATION_H__
#define __TERRAIN_MANIPULATION_H__

struct slab* _move_full_down(struct slab* initial_slab);

struct slab* _move_full_right(struct slab* initial_slab);

struct slab* _initiate_new_slab();

struct terrain* _initialize_new_terrain();

void _load_terrain(struct terrain* t);

struct slab* _move(struct slab* current_slab, int direction);

void _set_warp(struct terrain* t);

struct terrain* initiate_terrain();

#endif