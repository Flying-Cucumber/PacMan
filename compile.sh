#!/usr/bin/env bash

gcc -Wall -g chase.c entity_manip.c graphics.c terrain_manipulation.c main.c `sdl-config --cflags --libs` -o pacman.x