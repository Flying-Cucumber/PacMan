#include <SDL/SDL.h>
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

void pause();

int start_interface(struct game* g){
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

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