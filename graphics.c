#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "game.h"
#include "graphics.h"
#include "game_constants.h"
#include "terrain_manipulation.h"
#include <stdlib.h>
#include <stdio.h>

int start_interface(SDL_Surface* background, Game* g){

    paint_terrain(background, g->t);
    //paint_entities(background, g);
    paint_ghost(background, g);
    g->p->self->affichage = draw_rectangle(background, g->p->self->current_slab->x * SLAB_SIZE, g->p->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 255, 0);


    return EXIT_SUCCESS;
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
    
    return rectangle;

}

void paint_terrain(SDL_Surface* background, Terrain* t){ //Parcourt toute les slab pour les afficher à l'écran
    Slab* current_slab = t->initial_slab;
    Slab* top_slab;

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

void blit_slab(Slab** slabs_to_repaint, SDL_Surface* ecran){
    SDL_Rect pos;
    SDL_Rect obj_pos;
    pos.x = slabs_to_repaint[0]->x * SLAB_SIZE;
    pos.y = slabs_to_repaint[0]->y * SLAB_SIZE;
    switch (slabs_to_repaint[0]->type){
        case WALL:
            slabs_to_repaint[0]->affichage = draw_rectangle(ecran, slabs_to_repaint[0]->x * SLAB_SIZE, slabs_to_repaint[0]->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 255);
            slabs_to_repaint[0]->objet = NULL;
            break;
        case PAC_GUM:
            slabs_to_repaint[0]->affichage = draw_rectangle(ecran, slabs_to_repaint[0]->x * SLAB_SIZE, slabs_to_repaint[0]->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            slabs_to_repaint[0]->objet = draw_rectangle(ecran, ((3 * slabs_to_repaint[0]->x + 1 ) * SLAB_SIZE) / 3, ((3 * slabs_to_repaint[0]->y + 1 ) * SLAB_SIZE) / 3, SLAB_SIZE / 3, SLAB_SIZE / 3, 255, 255, 255);
            break;
        case SUPER_PAC_GUM:
            slabs_to_repaint[0]->affichage = draw_rectangle(ecran, slabs_to_repaint[0]->x * SLAB_SIZE, slabs_to_repaint[0]->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            slabs_to_repaint[0]->objet = draw_rectangle(ecran, ((5 * slabs_to_repaint[0]->x + 1) * SLAB_SIZE) / 5, ((5 * slabs_to_repaint[0]->y + 1) * SLAB_SIZE) / 5, 3 * SLAB_SIZE / 5, 3 * SLAB_SIZE / 5, 255, 255, 255);
            break;
        default:
            slabs_to_repaint[0]->affichage = draw_rectangle(ecran, slabs_to_repaint[0]->x * SLAB_SIZE, slabs_to_repaint[0]->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            slabs_to_repaint[0]->objet = NULL;
            break;
    }
    SDL_BlitSurface(slabs_to_repaint[0]->affichage, NULL, ecran, &pos);
    SDL_BlitSurface(slabs_to_repaint[0]->objet, NULL, ecran, &pos);

}

void draw_slab(SDL_Surface* background, Slab* current_slab){ //Représente les slab selon leur type

    switch (current_slab->type){
        case WALL:
            current_slab->affichage = draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 255);
            current_slab->objet = NULL;
            break;
        case PAC_GUM:
            current_slab->affichage = draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            current_slab->objet = draw_rectangle(background, ((3 * current_slab->x + 1 ) * SLAB_SIZE) / 3, ((3 * current_slab->y + 1 ) * SLAB_SIZE) / 3, SLAB_SIZE / 3, SLAB_SIZE / 3, 255, 255, 255);
            break;
        case SUPER_PAC_GUM:
            current_slab->affichage = draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            current_slab->objet = draw_rectangle(background, ((5 * current_slab->x + 1) * SLAB_SIZE) / 5, ((5 * current_slab->y + 1) * SLAB_SIZE) / 5, 3 * SLAB_SIZE / 5, 3 * SLAB_SIZE / 5, 255, 255, 255);
            break;
        default:
            current_slab->affichage = draw_rectangle(background, current_slab->x * SLAB_SIZE, current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
            current_slab->objet = NULL;
            break;
    }
}

void paint_pacman(SDL_Surface* background, Pacman* p, struct slab* p_previous_slab){

    p_previous_slab->affichage = draw_rectangle(background, p_previous_slab->x * SLAB_SIZE, p_previous_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 0, 0);
    p->self->affichage = draw_rectangle(background, p->self->current_slab->x * SLAB_SIZE, p->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 255, 0);
}

void paint_ghost(SDL_Surface* background, Game* g){

    g->blinky->self->affichage = draw_rectangle(background, g->blinky->self->current_slab->x * SLAB_SIZE, g->blinky->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 0, 0);
    g->inky->self->affichage = draw_rectangle(background, g->inky->self->current_slab->x * SLAB_SIZE, g->inky->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 255, 0);
    g->pinky->self->affichage = draw_rectangle(background, g->pinky->self->current_slab->x * SLAB_SIZE, g->pinky->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 0, 255);
    g->clyde->self->affichage = draw_rectangle(background, g->clyde->self->current_slab->x * SLAB_SIZE, g->clyde->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 255, 0);

}

void paint_entities(SDL_Surface* background, Game* g, Slab* p_previous_slab){

    paint_pacman(background, g->p, p_previous_slab);
    paint_ghost(background, g);
    
}