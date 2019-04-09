#include <float.h>
#include <stdbool.h>
#include <math.h>
#include "terrain.h"
#include "entities.h"
#include "entity_manip.h"
#include "chase.h"

/*** Chasing strategies of the four ghosts ***
* Each ghost has its own method which acts directly
* on that ghost's direction.
*
* The functions below only modify the ghosts' dir
* and target attribute, they don't actually move them.
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
float compute_distance(struct slab* slab_1, struct slab* slab_2) {
    /* Usual 2D cartesian distance */
    int x_diff = xSalbVect(slab_1, slab_2);
    int y_diff = ySalbVect(slab_1, slab_2);
    return (sqrtf((float)(x_diff*x_diff) + (float)(y_diff*y_diff)));
}

int get_dir(struct entity* e, struct slab* target){
    /* e is the ghost's entity. Returns the next direction. A valid
    * direction should always exist. Nevertheless, returns -1 if no
    * direction is found. */
    struct slab* slab = e->current_slab;
    int previous_dir = e->dir;
    float min_dist = FLT_MAX;
    float d;
    int dir = -1;
    if (slab->up->type == PATH && previous_dir != DOWN) {
        min_dist = compute_distance(target, slab->up);
        dir = UP;
    }
    if (slab->right->type == PATH && previous_dir != LEFT) {
        d = compute_distance(target, slab->right);
        if (d < min_dist) {min_dist = d; dir = RIGHT;}
    }
    if (slab->down->type == PATH && previous_dir != UP) {
        d = compute_distance(target, slab->down);
        if (d < min_dist) {min_dist = d; dir = DOWN;}
    }
    if (slab->left->type == PATH && previous_dir != RIGHT) {
        d = compute_distance(target, slab->left);
        if (d < min_dist) {dir = LEFT;}
    }
    return (dir);
}

struct slab* fieldBrowsing(struct slab* slab, int dir, unsigned int n){
    /* Moves n slabs in the direction dir, starting from slab.
    * Trying to return an non-existent slab will result in 
    * fieldBrowsing returning the current slab */
    if (n==0){return slab;}
    switch (dir){
        case (UP) :
            if (slab->up == NULL) {return slab;}
            return (fieldBrowsing(slab->up, UP, n-1));
        case (RIGHT) :
            if (slab->right == NULL) {return slab;}
            return (fieldBrowsing(slab->right, RIGHT, n-1));
        case (DOWN) :
            if (slab->down == NULL) {return slab;}
            return (fieldBrowsing(slab->down, DOWN, n-1));
        default :
            if (slab->left == NULL) {return slab;}
            return (fieldBrowsing(slab->left, LEFT, n-1));
    }
}

bool rhino_Pinky(int pcm_dir, int pink_dir, struct slab* pcm_slab, 
                 struct slab* pink_slab){
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
    if ((pink_x != pcm_x || abs(pink_y - pcm_y) >= 3*SLAB_SIZE) &&
       (pink_y != pcm_y || abs(pink_x - pcm_x) >= 3*SLAB_SIZE)){return false;}

    // Amongst 12 possibilities, 9 are unwanted (and unlikely):
    // check if Pac-Man and Pinky are face-to-face
    int i=1;
    bool pacman_is_incoming = false;
    while (i<4){
        struct slab* incoming_slab = fieldBrowsing(pink_slab, pink_dir, i);
        if (incoming_slab->type != PATH){break;}
        if (pcm_slab == incoming_slab) {pacman_is_incoming = true;}
        i++;
    }
    if (pacman_is_incoming){return (true);}
    return (false);
}

int xSlabVect(struct slab* slab_tail, struct slab* slab_head){
    /* Returns the x part of the vector slab_head - slab_tail */
    return (slab_head->x - slab_tail->x);
}

int ySlabVect(struct slab* slab_tail, struct slab* slab_head){
    /* Returns the y part of the vector slab_head - slab_tail */
    return (slab_head->y - slab_tail->y);
}

struct slab* getSlabFromXY(struct slab* o_slab, int vect_x, int vect_y){
    /* Returns the closest slab from the position o_slab + (vect_x, vect_y) */
    int xdir, ydir;
    if (vect_x > 0) {xdir = RIGHT;}
    else {xdir = LEFT;}
    if (vect_y > 0) {ydir = DOWN;}
    else {ydir = UP;}
    // fieldBrowsing already handles the bounds of the field
    return (fieldBrowsing(fieldBrowsing(o_slab, xdir, vect_x), ydir, vect_y));
}

//////////////////////////////// Main methods ////////////////////////////////

// Modify Blinky and Inky target slabs !

void chase_Blinky(struct ghost Blinky, struct pacman pacman){
    /* Returns Blinky's new direction.
    * Blinky is ruthless guy. He is the most agressive ghost 
    * and always targets the current Pac-Man slab */
    // There always exist a valid direction, no need to consider -1
    Blinky.self->dir = get_dir(Blinky.self, pacman.self->current_slab);
}

void chase_Pinky(struct ghost Pinky, struct pacman pacman){
    /* Returns Pinky's new direction.
    * Pinky is the smart guy. He tries to get in front of 
    * Pac-Man targeting a few slabs ahead of him */
    int pcm_dir = pacman.self->dir;
    int pink_dir = Pinky.self->dir;
    struct slab* pcm_slab = pacman.self->current_slab;

    // Check if Pinky must go on rhino mode
    if (rhino_Pinky(pcm_dir, pink_dir, pcm_slab, Pinky.self->current_slab)){
        // Rhino mode is nothing more than Blinky's normal behavior...
        Pinky.self->dir = get_dir(Pinky.self, pcm_slab);
    }
    // Else, standard Pinky behavior
    else{
        // Pinky targets 3 slabs ahead of Pac-Man
        struct slab* target = fieldBrowsing(pcm_slab, pcm_dir, 3);
        Pinky.self->dir = get_dir(Pinky.self, target);
    }
}

void chase_Inky(struct ghost Inky, struct ghost Blinky, struct pacman pacman){
    /* Returns Inky's new direction.
    * Inky is the complicated guy. He targets a slab given by
    * both Pac-Man's and Blinky's slabs. */
    int pcm_dir = pacman.self->dir;
    struct slab* pcm_slab = pacman.self->current_slab;
    struct slab* blink_slab = Blinky.self->current_slab;
    // Get 2 slabs ahead of Pac-Man
    struct slab* temp_target = fieldBrowsing(pcm_slab, pcm_dir, 2);
    // Get the vector from Blinky to that temp_target
    int vect_x = xSalbVect(blink_slab, temp_target);
    int vect_y = ySlabVect(blink_slab, temp_target);
    // Target slab is given by twice that vector
    struct slab* target = getSlabFromXY(blink_slab, 2*vect_x, 2*vect_y);
    Inky.target = target;
    Inky.self->dir = get_dir(Inky.self, target);
}