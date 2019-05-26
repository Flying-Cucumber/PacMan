#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "game.h"

int start_interface(SDL_Surface* background, Game* g);
SDL_Surface* draw_rectangle(SDL_Surface* background, int coord_x, int coord_y, int size_x, int size_y, int color_red, int color_green, int color_blue);
void paint_terrain(SDL_Surface* background, Terrain* t);
void draw_slab(SDL_Surface* background, Slab* current_slab);
void paint_pacman(SDL_Surface* background, Pacman* p, Slab* previous_slab);
void paint_ghost(SDL_Surface* background, Game* g);
void paint_entities(SDL_Surface* background, Game* g, Slab* p_previous_slab);
void entity_anim(Entity* e, int begin_anim, int end_anim, int pos_x, int pos_y);
SDL_Surface* crop_surface(int n);
Animation* build_animation(int begining_sprite, int ending_sprite, int pos_x, int pos_y);
void display_animation(SDL_Surface* background, Animation* animation);
void initialize_anim(Game* g);
void entity_anim(Entity* e, int begin_anim, int end_anim, int pos_x, int pos_y);
void refresh_entity(Entity* e, SDL_Surface* background);
void refresh_entities(SDL_Surface* background, Game* g, Slab** slabs_to_repaint);

#endif