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

struct terrain* load_terrain(){ // On va lire un fichier texte dans lequel le terrain est défini
    
    FILE *f;
    f = fopen("terrain.txt", "r");  // Ouverture du fichier
    
    struct terrain* t = malloc(sizeof(struct terrain)); // Création du terrain
    struct slab* current_slab = t->initial_slab;    // Initialisation de l'algorithme de génération
    char c = fgetc(f);
    int x = 0, y = 0;

    while (c != EOF){
        struct slab* new_slab = malloc(sizeof(struct slab));
        if (c == "\n"){ // Cas de retour à la ligne dans le fichier
            current_slab = move_full_down(t->initial_slab);
            y += 1;
            x = 0;
        else{   // Autres cas
            x += 1;
        }
        }else{
            if (x > 0){ // En ce cas, la nouvelle case est placée à droite de la nouvelle case
                current_slab->right = new_slab;
                new_slab->left = current_slab;
                if (y > 0){ // En ce cas, il y a des cases au dessus, il faut donc les lier
                    new_slab->up = current_slab->up->right;
                    current_slab->up->right->down = new_slab; // On aurait pu utiliser "new_slab->up->down", mais c'eut été moins visuel
                }
            }else{  // En ce cas, on a créé une case qu'on va placer en dessous de la case actuelle
                current_slab->down = new_slab;
                new_slab->up = current_slab;
            }
        }
        new_slab.x = x;
        new_slab.y = y;
        new_slab.type = (int) c
        c = fgetc(f);
    }
    return t;
}