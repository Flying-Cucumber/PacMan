#ifndef __GAME_H__
#define __GAME_H__


////////////////////////////////////////////////////
/////////////// Entities' structures ///////////////
////////////////////////////////////////////////////

/* The basic object is "entity". It is shared
* by both Pac-Man and the ghosts */
struct entity{
    int speed;
    struct slab* current_slab;
    int dir;
};


/* Pac-Man has additionally a supersaiyen
* mode: when he can eat the ghosts. */
struct pacman{
    struct entity* self;
};

/* The ghosts are always trying to reach a
* specific slab : their "target" attribute */
struct ghost{
    struct entity* self;
    struct slab* target;
    int state;
};

struct fruit{
    struct entity* self;
    int points;
};


////////////////////////////////////////////////////
/////////////// Terrain's structures ///////////////
////////////////////////////////////////////////////


struct slab{    // Structure visant à modéliser des "cases", et permettant de naviguer facilement vers la case du dessus, dessous etc, et de gérer les interactions terrain-entités
    struct slab* up;
    struct slab* down;
    struct slab* left;
    struct slab* right;
    int type;
    int x;
    int y;
};

struct terrain{ // Structure englobant les diffrentes dalles, permettant de construire et lier les cases, puis de connaître la case de spawn de pacman et des fruits, ainsi que la maison des fantômes
    struct slab* initial_slab;
    struct slab* spawn_slab;
    struct slab* ghost_house;
};


////////////////////////////////////////////////////
//////////////// Game's structures /////////////////
////////////////////////////////////////////////////

struct game{    // Structure englobant tout les éléments du jeu, permettant d'accéder facilement à chacun. Utile pour gérer les interactions, et pour l'affichage graphique.
    struct terrain* t;
    struct pacman* p;
    struct ghost* pinky;
    struct ghost* blinky;
    struct ghost* inky;
    struct ghost* clyde;
    struct fruit* f;
    int score;
    int lives;
};

#endif