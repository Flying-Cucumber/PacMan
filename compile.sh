#!/usr/bin/env bash

gcc -Wall ~/Documents/ENSTA/IN104/PacMan/chase.c ~/Documents/ENSTA/IN104/PacMan/entity_manip.c ~/Documents/ENSTA/IN104/PacMan/graphics.c ~/Documents/ENSTA/IN104/PacMan/terrain_manipulation.c ~/Documents/ENSTA/IN104/PacMan/main.c `sdl-config --cflags --libs` -o ~/Documents/ENSTA/IN104/PacMan/pacman.x