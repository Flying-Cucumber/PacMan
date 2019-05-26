#ifndef __GAME_H__
#define __GAME_H__

#include <SDL/SDL.h>
//#include "graphics.h"


////////////////////////////////////////////////////
/////////////// Graphical structures ///////////////
////////////////////////////////////////////////////

typedef struct sprite{
    SDL_Surface* image_display;
    struct sprite* next_sprite;
} Sprite;

typedef struct animation{
    SDL_Rect position;
    struct sprite* current_sprite;
    struct sprite* first_sprite;
}Animation;


////////////////////////////////////////////////////
/////////////// Entities' structures ///////////////
////////////////////////////////////////////////////

/* The basic object is "entity". It is shared
* by both Pac-Man and the ghosts */
typedef struct entity{
    struct slab* current_slab;
    int dir;
    Animation* anim_up;
    Animation* anim_down;
    Animation* anim_right;
    Animation* anim_left;
}Entity;


/* Pac-Man */
typedef struct pacman{
    struct entity* self;
}Pacman;

/* The ghosts are always trying to reach a
* specific slab : their "target" attribute */
typedef struct ghost{
    struct entity* self;
    // struct slab* target; // Useless !?!
    int state;
}Ghost;

typedef struct fruit{
    struct entity* self;
    int points;
}Fruit;

////////////////////////////////////////////////////
/////////////// Terrain's structures ///////////////
////////////////////////////////////////////////////


typedef struct slab{ // Structure visant à modéliser des "cases", et permettant de naviguer facilement vers la case du dessus, dessous etc, et de gérer les interactions terrain-entités
    struct slab* up;
    struct slab* down;
    struct slab* left;
    struct slab* right;
    int type;
    int x;
    int y;
    SDL_Surface* affichage;
    SDL_Surface* objet;
}Slab;

typedef struct terrain{ // Structure englobant les diffrentes dalles, permettant de construire et lier les cases, puis de connaître la case de spawn de pacman et des fruits, ainsi que la maison des fantômes
    int size_x;
    int size_y;    
    struct slab* initial_slab;
    struct slab* spawn_slab;
    struct slab* ghost_house;
}Terrain;

////////////////////////////////////////////////////
//////////////// Game's structures /////////////////
////////////////////////////////////////////////////

typedef struct game{    // Structure englobant tout les éléments du jeu, permettant d'accéder facilement à chacun. Utile pour gérer les interactions, et pour l'affichage graphique.
    struct terrain* t;
    struct pacman* p;
    struct ghost* pinky;
    struct ghost* blinky;
    struct ghost* inky;
    struct ghost* clyde;
    struct fruit* f;
    int score;
    int lives;
}Game;

#endif