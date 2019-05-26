#include <SDL/SDL.h>
#include "game.h"
#include "graphics.h"
#include "game_constants.h"
#include "terrain_manipulation.h"
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////// Initialisation ///////////////////////////////

int start_interface(SDL_Surface* background, Game* g){

    paint_terrain(background, g->t);
    //paint_entities(background, g);
    initialize_anim(g);
    paint_ghost(background, g);
    g->p->self->anim_left->current_sprite->image_display = draw_rectangle(background, g->p->self->current_slab->x * SLAB_SIZE, g->p->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 255, 0);


    return EXIT_SUCCESS;
}

//////////////////////////////////// Utils ////////////////////////////////////

SDL_Surface* draw_rectangle(SDL_Surface* background, int coord_x, int coord_y, int size_x, int size_y, int color_red, int color_green, int color_blue){
    //Permet de représenter facilement des rectangles
    
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

////////////////////////////// Painting entities //////////////////////////////

void paint_pacman(SDL_Surface* background, Pacman* p){

    p->self->anim_left->current_sprite->image_display = draw_rectangle(background, p->self->current_slab->x * SLAB_SIZE, p->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 255, 0);
}

void paint_ghost(SDL_Surface* background, Game* g){

    g->blinky->self->anim_left->current_sprite->image_display = draw_rectangle(background, g->blinky->self->current_slab->x * SLAB_SIZE, g->blinky->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 0, 0);
    g->inky->self->anim_left->current_sprite->image_display = draw_rectangle(background, g->inky->self->current_slab->x * SLAB_SIZE, g->inky->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 0, 255, 0);
    g->pinky->self->anim_left->current_sprite->image_display = draw_rectangle(background, g->pinky->self->current_slab->x * SLAB_SIZE, g->pinky->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 0, 255);
    g->clyde->self->anim_left->current_sprite->image_display = draw_rectangle(background, g->clyde->self->current_slab->x * SLAB_SIZE, g->clyde->self->current_slab->y * SLAB_SIZE, SLAB_SIZE, SLAB_SIZE, 255, 255, 0);

}

void paint_entities(SDL_Surface* background, Game* g){

    paint_pacman(background, g->p);
    paint_ghost(background, g);
    
}

void repaint_entity_slabs(SDL_Surface* background, Slab** slabs_to_repaint){
    for(int i = 0; i < 4; i++){
        draw_slab(background, slabs_to_repaint[i]);    
    }
}

//////////////////////////////////////////////////////////////
//////////  Tentative d'intégration des animations  //////////
//////////////////////////////////////////////////////////////

SDL_Surface* crop_surface(int n){
    int x = n / 8;
    int y = n % 8;
    SDL_Surface* sprite_sheet = SDL_LoadBMP("Media/Pictures/sprite.bmp");
    if (sprite_sheet == NULL){
        printf("Error: couldn't initialize the sprite sheet!\n");
        exit(1);
    }
    SDL_Surface* surface = SDL_CreateRGBSurface(sprite_sheet->flags, SPRITE_SIZE, SPRITE_SIZE, sprite_sheet->format->BitsPerPixel, sprite_sheet->format->Rmask, sprite_sheet->format->Gmask, sprite_sheet->format->Bmask, sprite_sheet->format->Amask);
    SDL_Rect rect = {x * SPRITE_SIZE, y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE};
    SDL_BlitSurface(sprite_sheet, &rect, surface, 0);
    return surface;
}

Animation* build_animation(int begining_sprite, int ending_sprite, int pos_x, int pos_y){
    Animation* animation = malloc(sizeof(Animation));
    Sprite* old_sprite = NULL;
    animation->position.x = pos_x;
    animation->position.y = pos_y;
    for(int i = begining_sprite; i < ending_sprite; i++){
        Sprite* new_sprite = malloc(sizeof(new_sprite));
        new_sprite->image_display = crop_surface(i);
        new_sprite->next_sprite = old_sprite;
        old_sprite = new_sprite;
    }
    animation->first_sprite = old_sprite;
    while(old_sprite->next_sprite != NULL){
        old_sprite = old_sprite->next_sprite;
    }
    old_sprite->next_sprite = animation->first_sprite;
    animation->current_sprite = animation->first_sprite;
    return animation;
}

void display_animation(SDL_Surface* background, Animation* animation){
    animation->current_sprite = animation->current_sprite->next_sprite;
    SDL_BlitSurface(animation->current_sprite->image_display, NULL, background, &animation->position);
}

void initialize_anim(Game* g){
    entity_anim(g->p->self, 0, 11, g->p->self->current_slab->x * SLAB_SIZE, g->p->self->current_slab->y * SLAB_SIZE);
    entity_anim(g->blinky->self, 16, 23, g->blinky->self->current_slab->x * SLAB_SIZE, g->blinky->self->current_slab->y * SLAB_SIZE);
    entity_anim(g->pinky->self, 24, 31, g->pinky->self->current_slab->x * SLAB_SIZE, g->pinky->self->current_slab->y * SLAB_SIZE);
    entity_anim(g->inky->self, 32, 39, g->inky->self->current_slab->x * SLAB_SIZE, g->inky->self->current_slab->y * SLAB_SIZE);
    entity_anim(g->clyde->self, 40, 47, g->clyde->self->current_slab->x * SLAB_SIZE, g->clyde->self->current_slab->y * SLAB_SIZE);
}

void entity_anim(Entity* e, int begin_anim, int end_anim, int pos_x, int pos_y){
    int anim_lenght = (end_anim - begin_anim) / 4;
    e->anim_up = build_animation(begin_anim, begin_anim + anim_lenght, pos_x, pos_y);
    e->anim_down = build_animation(begin_anim + anim_lenght, begin_anim + 2 * anim_lenght, pos_x, pos_y);
    e->anim_left = build_animation(begin_anim + 2 * anim_lenght, begin_anim + 3 * anim_lenght, pos_x, pos_y);
    e->anim_right = build_animation(begin_anim + 3 * anim_lenght, end_anim, pos_x, pos_y);
}

void refresh_entity(Entity* e, SDL_Surface* background){
    switch (e->dir)
    {
    case UP:
        display_animation(background, e->anim_up);
        break;
    case DOWN:
        display_animation(background, e->anim_down);
        break;
    case RIGHT:
        display_animation(background, e->anim_right);
        break;
    case LEFT:
        display_animation(background, e->anim_left);
        break;
    default:
        break;
    }
}

void refresh_entities(SDL_Surface* background, Game* g){
    refresh_entity(g->p->self, background);
    refresh_entity(g->blinky->self, background);
    refresh_entity(g->pinky->self, background);
    refresh_entity(g->inky->self, background);
    refresh_entity(g->clyde->self, background);
}