#include <SDL/SDL.h>

void pause(){
    
    int game_on = 1;
    SDL_Event event;

    while (game_on){

        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                game_on = 0;
        }

    }
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Le PacMan de Télécom Sud Paris!", NULL);

    pause();

    SDL_Quit();
    return EXIT_SUCCESS;
}