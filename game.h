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
    SDL_Surface* affichage;
};

typedef struct entity Entity;


/* Pac-Man */
struct pacman{
    struct entity* self;
};

typedef struct pacman Pacman;

/* The ghosts are always trying to reach a
* specific slab : their "target" attribute */
struct ghost{
    struct entity* self;
    // struct slab* target; // Useless !?!
    int state;
};

typedef struct ghost Ghost;

struct fruit{
    struct entity* self;
    int points;
};

typedef struct fruit Fruit;

////////////////////////////////////////////////////
/////////////// Terrain's structures ///////////////
////////////////////////////////////////////////////


struct slab{ // Structure visant à modéliser des "cases", et permettant de naviguer facilement vers la case du dessus, dessous etc, et de gérer les interactions terrain-entités
    struct slab* up;
    struct slab* down;
    struct slab* left;
    struct slab* right;
    int type;
    int x;
    int y;
    SDL_Surface* affichage;
    SDL_Surface* objet;
};

typedef struct slab Slab;

struct terrain{ // Structure englobant les diffrentes dalles, permettant de construire et lier les cases, puis de connaître la case de spawn de pacman et des fruits, ainsi que la maison des fantômes
    int size_x;
    int size_y;    
    struct slab* initial_slab;
    struct slab* spawn_slab;
    struct slab* ghost_house;
};

typedef struct terrain Terrain;

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

typedef struct game Game;

#endif