#ifndef __GAME_H__
#define __GAME_H__

#include "terrain.h"
#include "entities.h"

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