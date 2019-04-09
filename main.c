#include <stdlib.h>
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
    printf("Chargement terminé\n");
    return 0;
}