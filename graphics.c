#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

void pause();

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){ 
    SDL_Rect offset;
	offset.x = x; 
	offset.y = y;
	SDL_BlitSurface(source, NULL, destination, &offset); 
}

int start_interface(struct game* g){

    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *screen = NULL;
    SDL_Surface *field = NULL;
    SDL_Surface *pacman_surf = NULL;
    SDL_Surface *blinky_surf = NULL;
    SDL_Surface *pinky_surf = NULL;
    SDL_Surface *inky_surf = NULL;
    SDL_Surface *clyde_surf = NULL; 

    field = IMG_Load("Media/Pictures/field.jpg");


    screen = SDL_SetVideoMode(520, 600, 32, SDL_HWSURFACE);
    if (screen == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Uint32 handsome_color = SDL_MapRGB(screen->format, 13, 26, 52);
    SDL_FillRect(screen, NULL, handsome_color);
    SDL_Flip(screen);

    apply_surface(0, 0, field, screen);

    SDL_WM_SetCaption("Pac-Man", NULL);

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