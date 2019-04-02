#ifndef __PACMAN_MANIPULATION_H__
#define __PACMAN_MANIPULATION_H__

#include <stdlib.h>
#include "terrain.h"
#include "entities.h"

void pacman_move_up(struct pacman *pcm){
    pcm->current_slab = pcm->current_slab->up;
}

void pacman_move_down(struct pacman *pcm){
    pcm->current_slab = pcm->current_slab->down;
}

void pacman_move_right(struct pacman *pcm){
    pcm->current_slab = pcm->current_slab->right;
}

void pacman_move_left(struct pacman *pcm){
    pcm->current_slab = pcm->current_slab->left;
}

#endif
