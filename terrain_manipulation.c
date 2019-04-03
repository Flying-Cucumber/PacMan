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

struct slab* initiate_new_slab(){   // Fonction de création de nouvelle dalle
    struct slab* new_slab = malloc(sizeof(struct slab));
    new_slab->down = NULL;
    new_slab->left = NULL;
    new_slab->right = NULL;
    new_slab->up = NULL;
    new_slab->type = 0;
    new_slab->x = 0;
    new_slab->y = 0;
    return new_slab;
}

struct terrain* initialize_new_terrain(){
    struct terrain* t = malloc(sizeof(struct terrain)); // Création du terrain
    t->initial_slab = initiate_new_slab();
    t->spawn_slab = NULL;
    return t;
}

void load_terrain(struct terrain* t){ // On va lire un fichier texte dans lequel le terrain est défini
    
    printf("ouverture du fichier\n");

    FILE *f;
    f = fopen("terrain.txt", "r");  // Ouverture du fichier
    
    printf("Lancement du chargement\n");

    struct slab* current_slab = t->initial_slab;    // Initialisation de l'algorithme de génération
    char c = fgetc(f);
    int x = 0, y = 0;

    printf("Variables correctement initialisées\n");

    while (c != EOF){
        
        if (c == '\n'){ // Cas de retour à la ligne dans le fichier
            current_slab = move_full_down(t->initial_slab);
            y += 1;
            x = 0;
        }else{
            struct slab* new_slab = initiate_new_slab();
            if (x > 0){ // En ce cas, la nouvelle case est placée à droite de la nouvelle case
                current_slab->right = new_slab;
                new_slab->left = current_slab;
                if (y > 0){ // En ce cas, il y a des cases au dessus, il faut donc les lier
                    current_slab->up->right->down = new_slab; // On aurait pu utiliser "new_slab->up->down", mais c'eut été moins visuel
                    new_slab->up = current_slab->up->right;
                    //printf("à droite: (%d, %d), en dessous:(%d, %d)", t->initial_slab->right->x, t->initial_slab->right->y, t->initial_slab->down->x, t->initial_slab->down->y);
                }
            }else{  // En ce cas, on a créé une case qu'on va placer en dessous de la case actuelle
                current_slab->down = new_slab;
                new_slab->up = current_slab;
                printf("Let's go down!\n");
            }
            new_slab->y = y;
            new_slab->x = x;
            new_slab->type = (int) c - 48;
            printf("Coordonnées: (%d, %d), type: %d\n", x, y, new_slab->type);
            current_slab = new_slab;
            x += 1;
        }
        c = fgetc(f);
    }
    printf("Chargement terminé\n");
}

struct slab* move(struct slab* current_slab, int direction){
    switch (direction)
    {
        case 4:
            if (current_slab->left != NULL){
                current_slab = current_slab->left;
            }
            break;
        case 2:
            if (current_slab->down != NULL){
                current_slab = current_slab->down;
            }
            break;
        case 6:
            if (current_slab->right != NULL){
                current_slab = current_slab->right;
            }
            break;
        case 8:
            if (current_slab->up != NULL){
                current_slab = current_slab->up;
            }
            break;
        default:
            break;
    }
    printf("(%d, %d), type: %d\n", current_slab->x, current_slab->y, current_slab->type);
    return current_slab;
}

struct terrain* set_warp(struct terrain* t){
    struct slab* horizontal_warp_1 = t->initial_slab;
    struct slab* vertical_warp_1 = t->initial_slab;
    while (horizontal_warp_1 != NULL){
        if (horizontal_warp_1->type == 1){
            struct slab* horizontal_warp_2 = move_full_down(horizontal_warp_1);
            
        }
    }
}


int main(){
    printf("Démarrage\n");
    printf("Initialisation\n");
    int i = 0;
    struct terrain* t = initialize_new_terrain();
    struct slab* current_slab = t->initial_slab;
    load_terrain(t);
    while (i != 5){
        scanf("%d", &i);
        current_slab = move(current_slab, i);
    };
    printf("done\n");
    return 0;
}