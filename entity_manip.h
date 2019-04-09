#ifndef __ENTITY_MANIP_H__
#define __ENTITY_MANIP_H__

#include <stdlib.h>
#include <stdbool.h>
#include "terrain.h"
#include "entities.h"

struct entity* entity_initiate(struct slab* current_slab);

struct pacman* pacman_initiate(struct slab* current_slab);

struct ghost* ghost_initiate(struct slab* current_slab);

void move_up(struct entity* e);

void move_down(struct entity* e);

void move_right(struct entity* e);

void move_left(struct entity* e);

#endif
