#ifndef __CHASE_H__
#define __CHASE_H__

#include <float.h>
#include <stdbool.h>
#include <math.h>
#include "game.h"
#include "game_constants.h"
#include "entity_manip.h"

int abs(int i);
int compute_dist_1(struct slab* slab_1, struct slab* slab_2);
int get_dir(struct entity* e, struct slab* target);
struct slab* fieldBrowsing(struct slab* slab, int dir, unsigned int n);
struct slab* terrain_Browsing(struct slab* slab, int dir, unsigned int n);
bool rhino_Pinky(int pcm_dir, int pink_dir, struct slab* pcm_slab, struct slab* pink_slab);
int xSlabVect(struct slab* slab_tail, struct slab* slab_head);
int ySlabVect(struct slab* slab_tail, struct slab* slab_head);
struct slab* getSlabFromXY(struct slab* o_slab, int vect_x, int vect_y);
void chase_Blinky(struct ghost* Blinky, struct pacman* pacman);
void chase_Pinky(struct ghost* Pinky, struct pacman* pacman);
void chase_Inky(struct ghost* Inky, struct ghost* Blinky, struct pacman* pacman);
void chase_mode(struct game* g, Slab** slabs_to_repaint);

#endif
