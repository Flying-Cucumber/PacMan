#ifndef __CHASE_H__
#define __CHASE_H__

#include <float.h>
#include <stdbool.h>
#include <math.h>
#include "terrain.h"
#include "entities.h"
#include "entity_manip.h"
#include "chase.h"

float compute_distance(struct slab* slab_1, struct slab* slab_2);
int get_dir(struct entity* e, struct slab* target);
struct slab* terrain_Browsing(struct slab* slab, int dir, unsigned int n);
bool rhino_Pinky(int pcm_dir, int pink_dir, struct slab* pcm_slab, struct slab* pink_slab);
void chase_Blinky(struct ghost Blinky);
void chase_Pinky(struct ghost Pinky);

#endif
