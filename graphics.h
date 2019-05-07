#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "game.h"

int start_interface(SDL_Surface* background, Game* g);
SDL_Surface* draw_rectangle(SDL_Surface* background, int coord_x, int coord_y, int size_x, int size_y, int color_red, int color_green, int color_blue);
void paint_terrain(SDL_Surface* background, Terrain* t);
void draw_slab(SDL_Surface* background, Slab* current_slab);
void paint_pacman(SDL_Surface* background, Pacman* p);
void paint_ghost(SDL_Surface* background, Game* g);
void paint_entities(SDL_Surface* background, Game* g);

#endif