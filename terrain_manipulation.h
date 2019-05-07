#ifndef __TERRAIN_MANIPULATION_H__
#define __TERRAIN_MANIPULATION_H__

#include "game.h"

struct terrain* initiate_terrain();
Slab* move_straight(Slab* initial_slab, int direction, int distance);
int Is_Path(struct slab* slab);

#endif