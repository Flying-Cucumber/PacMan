#include <SDL/SDL.h>
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

void pause();

int start_interface(struct game* g){

    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *ecran = NULL;

    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    if (ecran == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Uint32 handsome_color = SDL_MapRGB(ecran->format, 0, 255, 0);
    SDL_FillRect(ecran, NULL, handsome_color);
    SDL_Flip(ecran);

    SDL_WM_SetCaption("Intelligence artificielle pour Hanabi", NULL);

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