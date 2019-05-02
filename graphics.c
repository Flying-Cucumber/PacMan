#include <SDL/SDL.h>
#include "game.h"
#include "game_constants.h"
#include "terrain_manipulation.h"
#include <stdlib.h>
#include <stdio.h>

void pause();
SDL_Surface* draw_rectangle(SDL_Surface* background, int coord_x, int coord_y, int size_x, int size_y, int color_red, int color_green, int color_blue);
void paint_terrain(SDL_Surface* background, struct terrain* t);
void draw_slab(SDL_Surface* background, struct slab* current_slab);

int start_interface(struct game* g){

    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *ecran = NULL;


    ecran = SDL_SetVideoMode((g->t->size_x * SLAB_SIZE), (g->t->size_y * SLAB_SIZE), 32, SDL_HWSURFACE);
    if (ecran == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 0, 0));

    SDL_WM_SetCaption("Intelligence artificielle pour Hanabi", NULL);

    paint_terrain(ecran, g->t);

    pause();

    SDL_Quit();

    return EXIT_SUCCESS;
}

void pause(){
    int continuer = 1;
    SDL_Event event;

    while (continuer){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                continuer = 0;
        }
    }
}

SDL_Surface* draw_rectangle(SDL_Surface* background, int coord_x, int coord_y, int size_x, int size_y, int color_red, int color_green, int color_blue){ //Permet de représenter facilement des rectangles
    SDL_Rect position;
    position.x = coord_x;
    position.y = coord_y;
    SDL_Surface* rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, size_x, size_y, 32, 0, 0, 0, 0);
    if (rectangle == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(rectangle, NULL, SDL_MapRGB(background->format, color_red, color_green, color_blue));
    SDL_BlitSurface(rectangle, NULL, background, &position);
    SDL_Flip(background);
    return rectangle;
}

void paint_terrain(SDL_Surface* background, struct terrain* t){ //Parcoure toute les slab pour les afficher à l'écran
    struct slab* current_slab = t->initial_slab;
    struct slab* top_slab;

    while(current_slab != NULL){
        top_slab = current_slab;
        while(current_slab != NULL){

            draw_slab(background, current_slab);

            current_slab = current_slab->down;
        }
        current_slab = top_slab;
        current_slab = current_slab->right;
    }
}

void draw_slab(SDL_Surface* background, struct slab* current_slab){ //Représente les slab selon leur type
    switch (current_slab->type){
        case WALL:
            draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 255);
            break;
        case PAC_GUM:
            draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            draw_rectangle(background, (current_slab->x + (1 / 3)) * SLAB_SIZE, (current_slab->y + (1 / 3)) * SLAB_SIZE, SLAB_SIZE / 3, SLAB_SIZE / 3, 255, 255, 255);
            break;
        case SUPER_PAC_GUM:
            draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            draw_rectangle(background, (current_slab->x + (1 / 5)) * SLAB_SIZE, (current_slab->y + (1 / 5)) * SLAB_SIZE, 3 * SLAB_SIZE / 5, 3 * SLAB_SIZE / 5, 255, 255, 255);
            break;
        default:
            draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            break;
    }
    printf("Dalle (%d, %d) représentée", current_slab->x, current_slab->y);
}