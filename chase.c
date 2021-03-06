 #include <limits.h>
#include <stdbool.h>
#include "terrain_manipulation.h"
#include "entity_manip.h"
#include "game.h"
#include "chase.h"
#include "game_constants.h"

/*** Chasing strategies of the four ghosts ***
* Each ghost has its own method which acts directly
* on that ghost's direction.
*
*
* The strategy always consists in choosing the next
* directly accessible slab minimising the distance 
* to a target slab, which is ghost-specific. In case
* of "conflict", the order is: up>right>down>left.
*/

/*** A ghost may NEVER reverse its direction while in
* CHASE mode, which is always assumed here.
*/

//////////////////////////////////// Utils ////////////////////////////////////

int abs(int i){
    /* Returns the absolute value of i */
    if ((unsigned int) i == i) {
        return i;
    }else{
        return ((unsigned int) -1) ^ (((unsigned int) i) - 1);
    }
}

int compute_dist_1(Slab* slab_1, Slab* slab_2){
    return abs(slab_1->x - slab_2->x) + abs(slab_1->y - slab_2->y);
}

int xSlabVect(Slab* slab_tail, Slab* slab_head){
    /* Returns the x part of the vector slab_head - slab_tail */
    return (slab_head->x - slab_tail->x);
}

int ySlabVect(Slab* slab_tail, Slab* slab_head){
    /* Returns the y part of the vector slab_head - slab_tail */
    return (slab_head->y - slab_tail->y);
}

////////////////////////////// Secondary methods //////////////////////////////

int get_dir(Entity* e, Slab* target){
    /* e is the ghost's entity. Returns the next direction. A valid
    * direction should always exist. Nevertheless, prints an error and 
    * exits -1 if no direction is found. */
    Slab* slab = e->current_slab;
    int previous_dir = e->dir;
    int min_dist = INT_MAX;
    int d; // distance1 is always an integer!
    int dir = -1;
    if (Is_Path(slab->up) && previous_dir != DOWN) {
        min_dist = compute_dist_1(target, slab->up);
        dir = UP;
    }
    if (Is_Path(slab->right) && previous_dir != LEFT) {
        d = compute_dist_1(target, slab->right);
        if (d < min_dist) {min_dist = d; dir = RIGHT;}
    }
    if (Is_Path(slab->down) && previous_dir != UP) {
        d = compute_dist_1(target, slab->down);
        if (d < min_dist) {min_dist = d; dir = DOWN;}
    }
    if (Is_Path(slab->left) && previous_dir != RIGHT) {
        d = compute_dist_1(target, slab->left);
        if (d < min_dist) {dir = LEFT;}
    }
    if (dir == -1) {
        printf("Error in get_dir from chase.c: no valid direction found!\n");
        exit(-1);
    }
    return (dir);
}

bool rhino_Pinky(int pcm_dir, int pink_dir, Slab* pcm_slab, 
                 Slab* pink_slab){
    /* When Pinky is close enough from Pac-Man, and can rush over
    * him (<3 *PATH* slabs between them), he goes in rhino mode to 
    * prevent any avoidance behavior... 
    * Because the rhino mode is unlikely, faster preliminary checks are 
    * performed before the real one */

    // Check if directions are opposite
    if ((pcm_dir + 2) % 4 != pink_dir) {return false;}

    int pink_x = pink_slab->x;
    int pink_y = pink_slab->y;
    int pcm_x = pcm_slab->x;
    int pcm_y = pcm_slab->y;

    // Check if Pinky and Pac-Man are close on the same line
    if ((pink_x != pcm_x || abs(pink_y - pcm_y) >= 3) &&
       (pink_y != pcm_y || abs(pink_x - pcm_x) >= 3)){return false;}

    // Amongst 12 possibilities, 9 are unwanted (and unlikely):
    // check if Pac-Man and Pinky are face-to-face
    int i=1;
    bool pacman_is_incoming = false;
    while (i<4){
        Slab* incoming_slab = move_straight(pink_slab, pink_dir, i);
        if (incoming_slab->type != PATH){break;}
        if (pcm_slab == incoming_slab) {pacman_is_incoming = true;}
        i++;
    }
    if (pacman_is_incoming){return (true);}
    return (false);
}

Slab* getSlabFromXY(Slab* o_slab, int vect_x, int vect_y){
    /* Returns the closest slab from the position o_slab + (vect_x, vect_y) */
    int xdir, ydir;
    if (vect_x > 0) {xdir = RIGHT;}
    else {xdir = LEFT;}
    if (vect_y > 0) {ydir = DOWN;}
    else {ydir = UP;}
    // move_straight already handles the bounds of the field
    return (move_straight(move_straight(o_slab, xdir, vect_x), ydir, vect_y));
}

//////////////////////////////// Main methods ////////////////////////////////

// Modify the ghosts' dir attribute

void chase_Blinky(Ghost* Blinky, Pacman* pacman){
    /* Changes Blinky's direction.
    * Blinky is the ruthless guy. He is the most agressive ghost 
    * and always targets the current Pac-Man slab */
    Blinky->self->dir = get_dir(Blinky->self, pacman->self->current_slab);
}

void chase_Pinky(Ghost* Pinky, Pacman* pacman){
    /* Changes Pinky's direction.
    * Pinky is the smart guy. He tries to get in front of 
    * Pac-Man targeting a few slabs ahead of him */
    int pcm_dir = pacman->self->dir;
    int pink_dir = Pinky->self->dir;
    Slab* pcm_slab = pacman->self->current_slab;

    // Check if Pinky must go on rhino mode
    if (rhino_Pinky(pcm_dir, pink_dir, pcm_slab, Pinky->self->current_slab)){
        // Rhino mode is nothing more than Blinky's normal behavior...
        Pinky->self->dir = get_dir(Pinky->self, pcm_slab);
    }else{ // Else, standard Pinky behavior
        // Pinky targets 3 slabs ahead of Pac-Man
        Slab* target = move_straight(pcm_slab, pcm_dir, 3);
        Pinky->self->dir = get_dir(Pinky->self, target);
    }
}

void chase_Inky(Ghost* Inky, Ghost* Blinky, Pacman* pacman){
    /* Changes Inky's direction.
    * Inky is the complicated guy. He targets a slab given by
    * both Pac-Man's and Blinky's slabs. */
    int pcm_dir = pacman->self->dir;
    Slab* pcm_slab = pacman->self->current_slab;
    Slab* blink_slab = Blinky->self->current_slab;
    // Get 2 slabs ahead of Pac-Man
    Slab* temp_target = move_straight(pcm_slab, pcm_dir, 2);
    // Get the vector from Blinky to that temp_target
    int vect_x = xSlabVect(blink_slab, temp_target);
    int vect_y = ySlabVect(blink_slab, temp_target);
    // Target slab is given by twice that vector...
    Slab* target = getSlabFromXY(blink_slab, 2*vect_x, 2*vect_y);
    Inky->self->dir = get_dir(Inky->self, target);
}

void chase_mode(Game* g, Slab** slabs_to_repaint){
    /* Changes all ghosts' directions, then moves them */
    chase_Blinky(g->blinky, g->p);
    chase_Pinky(g->pinky, g->p);
    chase_Inky(g->inky, g->blinky, g->p);
    move_all_ghosts(g, slabs_to_repaint);
}