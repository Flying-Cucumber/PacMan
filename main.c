#include <stdlib.h>
#include <stdio.h>
#include "terrain.h"
#include "terrain_manipulation.h"
#include "entities.h"
#include "entity_manip.h"


int main(){
    struct terrain* t = initiate_terrain();
    struct pacman* p = pacman_initiate(t->spawn_slab);
    struct ghost* pinky = ghost_initiate(t->ghost_house);
    struct ghost* blinky = ghost_initiate(t->ghost_house);
    struct ghost* inky = ghost_initiate(t->ghost_house);
    struct ghost* clyde = ghost_initiate(t->ghost_house);
    printf("Chargement terminé, Pacman en (%d, %d), Pinky en (%d, %d), Blinky en (%d, %d), Inky en (%d, %d) et Clyde en (%d, %d)\n", p->self->current_slab->x, p->self->current_slab->y, pinky->self->current_slab->x, pinky->self->current_slab->y, blinky->self->current_slab->x, blinky->self->current_slab->y, inky->self->current_slab->x, inky->self->current_slab->y, clyde->self->current_slab->x, clyde->self->current_slab->y);
    return 0;
}