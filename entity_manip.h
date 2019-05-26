#ifndef __ENTITY_MANIP_H__
#define __ENTITY_MANIP_H__

#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "terrain_manipulation.h"
#include "game.h"

Entity* entity_initiate(Slab* current_slab);

struct pacman* pacman_initiate(Slab* current_slab);

struct ghost* ghost_initiate(Slab* current_slab);

void move_up(Entity* e);

void move_down(Entity* e);

void move_right(Entity* e);

void move_left(Entity* e);

Slab* ghost_move(Entity* e);

void move_all_ghosts(Game* g, Slab** slabs_to_repaint){

void set_dir(SDLKey pressed, Entity* e);

void pacman_move(Pacman* p, int direction, Slab** slabs_to_repaint);

void entity_animations_update(Entity* e);

void pacman_interaction(Game* g);

#endif
