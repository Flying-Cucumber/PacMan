#include <stdio.h>
#include <stdlib.h>
#include "terrain.h"

struct slab* move_full_down(struct slab* initial_slab){
    struct slab* s = initial_slab;
    while (s->down != NULL){
        s = s->down;
    }
    return s;
}

struct terrain* load_terrain(){
    FILE *f;
    f = fopen("terrain.txt", "r");
    struct terrain* t = malloc(sizeof(struct terrain));
    struct slab* current_slab = t->initial_slab;
    char* c;
    while ((c = fgetc(f)) != EOF){
        if (c == "\n"){
            current_slab = t->initial_slab;
        }
        
    }
    return t;
}