#include <float.h>
#include <stdbool.h>
#include <math.h>
#include "terrain.h"
#include "entities.h"
#include "entity_manip.h"
#include "chase.h"

/*** Chasing strategies of the four ghosts ***
* Each ghost has its own method which acts directly
* on that ghost.
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
    int x_diff = (slab_1->_x_middle - slab_2->_x_middle);
    int y_diff = (slab_1->_y_middle - slab_2->_y_middle);
    return (sqrtf((float)(x_diff*x_diff) + (float)(y_diff*y_diff)));
}

int get_dir(struct entity* e, struct slab* target){
    /* e is the ghost's entity. Returns the next direction. If no direction
    * is found, returns -1, which must be handled outside the func */
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

struct slab* terrain_Browsing(struct slab* slab, int dir, unsigned int n){
    /* Moves n slabs in the direction dir, starting from slab */
    if (n==0){return slab;}
    switch (dir){
        case (UP) :
            return (terrain_Browsing(slab->up, UP, n-1));
        case (RIGHT) :
            return (terrain_Browsing(slab->right, RIGHT, n-1));
        case (DOWN) :
            return (terrain_Browsing(slab->down, DOWN, n-1));
        default :
            return (terrain_Browsing(slab->left, LEFT, n-1));
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

    int pink_x = pink_slab->_x_middle;
    int pink_y = pink_slab->_y_middle;
    int pcm_x = pcm_slab->_x_middle;
    int pcm_y = pcm_slab->_y_middle;

    // Check if Pinky and Pac-Man are close on the same line
    if ((pink_x != pcm_x || abs(pink_y - pcm_y) >= 3*SLAB_SIZE) &&
       (pink_y != pcm_y || abs(pink_x - pcm_x) >= 3*SLAB_SIZE)){return false;}

    // Amongst 12 possibilities, 9 are unwanted (and unlikely):
    // check if Pac-Man and Pinky are face-to-face
    int i=1;
    bool pacman_is_incoming = false;
    while (i<4){
        struct slab* incoming_slab = terrain_Browsing(pink_slab, pink_dir, i);
        if (incoming_slab->type != PATH){break;}
        if (pcm_slab == incoming_slab) {pacman_is_incoming = true;}
        i++;
    }
    if (pacman_is_incoming){return (true);}
    return (false);
}

//////////////////////////////// Main methods ////////////////////////////////

void chase_Blinky(struct ghost Blinky, struct pacman pacman){
    /* Returns Blinky's new direction.
    * Blinky is the red ghost. He is the most agressive ghost 
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
        struct slab* target = terrain_Browsing(pcm_slab, pcm_dir, 3);
        int new_dir = get_dir(Pinky.self, target);
        // We shall allow Pinky to target unavailable slabs for better 
        // behavior! For now, it behaves like Blinky...
        if (new_dir == -1) {Pinky.self->dir = get_dir(Pinky.self, pcm_slab);}
        else{Pinky.self->dir = new_dir;}
    }
}