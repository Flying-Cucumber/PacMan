#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "terrain.h"
#include "entities.h"

struct slab* _move_full_down(struct slab* initial_slab){
    struct slab* s = initial_slab;
    while (s->down != NULL){
        s = s->down;
    }
    return s;
}

struct slab* _move_full_right(struct slab* initial_slab){
    struct slab* s = initial_slab;
    while (s->right != NULL){
        s = s->right;
    }
    return s;
}

struct slab* _initiate_new_slab(){   // Fonction de création de nouvelle dalle
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

struct terrain* _initialize_new_terrain(){
    struct terrain* t = malloc(sizeof(struct terrain)); // Création du terrain
    t->initial_slab = _initiate_new_slab();
    t->spawn_slab = NULL;
    return t;
}

void _load_terrain(struct terrain* t){ // Fonction principale d'initialisation du terrain

    /*
    Le terrain défini dans "terrain.txt", placé à la racine du projet, doit être défini comme suit:

        - Des 0 pour marquer les murs
        - Des 1 pour marquer les cases classiques vides
        - Des 2 pour marquer les cases avec des pack gum
        - Des 3 pour les cases avec super pack gum
        - Un 4 pour la case d'apparition
        - Des 5 pour la maison des fantômes
        - Un 1 dans la première colonne du document créera automatiquement une warpzone sur la même ligne dans la dernière colonne du document
        - Un 1 sur la première ligne du document créera automatiquement une warpzone dans la même colonne sur la dernière ligne du document
        - Il ne doit pas y avoir de retour à la ligne à la fin de la dernière ligne

    */
    
    bool has_spawn = false;
    bool has_ghost_house = false;

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
            current_slab = _move_full_down(t->initial_slab);
            y += 1;
            x = 0;
        }else{
            struct slab* new_slab = _initiate_new_slab();
            
            // On cherche à rattacher la dalle aux dalles créées précédemment en fonction de sa position
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
            }

            // Initialisation des champs de la nouvelle dalle
            new_slab->y = y;
            new_slab->x = x;
            new_slab->type = (int) c - 48;

            if (new_slab->type == 4){
                t->spawn_slab = new_slab;
                has_spawn = true;
            }

            if (new_slab->type == 5){
                t->ghost_house = new_slab;
                has_ghost_house = true;
            }
            
            printf("Dalle créée: (%d, %d), type: %d\n", x, y, new_slab->type);
            
            current_slab = new_slab;
            
            x += 1;
        }

        c = fgetc(f);

    }

    printf("Chargement terminé\n");

    if (!has_spawn){
        printf("Warning: No spawn defined");
    }
    if (!has_ghost_house){
        printf("Warning: No ghost house defined");
    }

}

struct slab* _move(struct slab* current_slab, int direction){    // Pour se déplacer sur le terrain à l'aide du pavé numérique.
    switch (direction)
    {
        case LEFT:
            if (current_slab->left != NULL){
                current_slab = current_slab->left;
            }
            break;
        case DOWN:
            if (current_slab->down != NULL){
                current_slab = current_slab->down;
            }
            break;
        case RIGHT:
            if (current_slab->right != NULL){
                current_slab = current_slab->right;
            }
            break;
        case UP:
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

void _set_warp(struct terrain* t){   // Lie les tunnels aux extrémités 
    
    printf("Setting warps:\n");
    
    struct slab* horizontal_warp_1 = t->initial_slab;
    struct slab* vertical_warp_1 = t->initial_slab;
    
    while (horizontal_warp_1 != NULL){
        if (horizontal_warp_1->type == 1){
            
            // Lien des warpzones verticalement
            struct slab* horizontal_warp_2 = _move_full_down(horizontal_warp_1);
            horizontal_warp_2->type = 1;
            horizontal_warp_1->up = horizontal_warp_2;
            horizontal_warp_2->down = horizontal_warp_1;
            printf("Warp vertical créé entre (%d, %d) et (%d, %d)\n", horizontal_warp_1->x, horizontal_warp_1->y, horizontal_warp_2->x, horizontal_warp_2->y);

        }

        horizontal_warp_1 = horizontal_warp_1->left;

    }
    while (vertical_warp_1 != NULL){
        if (vertical_warp_1->type == 1){

            // Lien des warpzones horizontal
            struct slab* vertical_warp_2 = _move_full_right(vertical_warp_1);
            vertical_warp_2->type = 1;
            vertical_warp_1->left = vertical_warp_2;
            vertical_warp_2->right = vertical_warp_1;
            printf("Warp horizontal créé entre (%d, %d) et (%d, %d)\n", vertical_warp_1->x, vertical_warp_1->y, vertical_warp_2->x, vertical_warp_2->y);

        }

        vertical_warp_1 = vertical_warp_1->down;

    }

    printf("Warps setting done\n");

}


struct terrain* initiate_terrain(){
    printf("Démarrage\n");
    printf("Initialisation\n");

    struct terrain* t = _initialize_new_terrain();   // Initializing terrain
    _load_terrain(t);
    _set_warp(t);

    /*int i = 0;
    struct slab* current_slab = t->initial_slab;
    
    while (i != 5){ // Exploration du terrain pour tester les différentes structures
        scanf("%d", &i);
        current_slab = _move(current_slab, i);
    };*/
    
    printf("Done\n");
    
    return t;
}