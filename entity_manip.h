#ifndef __ENTITY_MANIP_H__
#define __ENTITY_MANIP_H__

#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "terrain_manipulation.h"
#include "game.h"

struct entity* entity_initiate(struct slab* current_slab);

struct pacman* pacman_initiate(struct slab* current_slab);

struct ghost* ghost_initiate(struct slab* current_slab);

void move_up(struct entity* e);

void move_down(struct entity* e);

void move_right(struct entity* e);

void move_left(struct entity* e);

Slab* move(struct entity* e);

void move_all_ghosts(struct game* g, Slab** slabs_to_repaint);

void set_dir(SDLKey pressed, struct entity* e);

void pacman_move(Pacman* p, int direction, Slab** slabs_to_repaint);

void pacman_interaction(Game* g);

void animations_update(Entity* e);

void update_all_animations(Game* g);
#endif
