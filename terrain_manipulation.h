#ifndef __TERRAIN_MANIPULATION_H__
#define __TERRAIN_MANIPULATION_H__

struct slab* move_full_down(struct slab* initial_slab);

struct slab* move_full_right(struct slab* initial_slab);

struct slab* initiate_new_slab();

struct terrain* initialize_new_terrain();

void load_terrain(struct terrain* t);

struct slab* move(struct slab* current_slab, int direction);

void set_warp(struct terrain* t);

struct terrain* initiate_terrain();

#endif