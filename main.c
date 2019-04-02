#include <stdlib.h>
#include "terrain.h"
#include "entities.h"

<<<<<<< HEAD
void pacman_move_up(pcm){

=======
void pacman_move_up(struct pacman* pcm){
    pcm->current_slab = pcm->current_slab->up;
>>>>>>> 170b4042ef084581f9cbf43a0529800a1ebdba32
}

int main(){
    return 0;
}