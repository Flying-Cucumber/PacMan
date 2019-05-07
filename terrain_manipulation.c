#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "game.h"
#include "game_constants.h"

Slab* move_straight(Slab* initial_slab, int direction, int distance){ // set distance to FULL for moving straight until meeting the edge of the terrain
    
    Slab* s = initial_slab;
    Slab* s_next = s;
    distance ++;

    while (s_next != NULL && distance != 0){
        s = s_next;
        switch (direction){
            case UP:
                s_next = s->up;
                break;
            case DOWN:
                s_next = s->down;
                break;
            case RIGHT:
                s_next = s->right;
                break;
            case LEFT:
                s_next = s->left;
                break;
            default:
                break;        
        }
        distance--;
    }
    
    return s;
}

Slab* _initiate_new_slab(){   // Fonction de création de nouvelle dalle
    
    Slab* new_slab = malloc(sizeof(Slab));
    if (new_slab == NULL){
        exit(0);
    }
    
    new_slab->down = NULL;
    new_slab->left = NULL;
    new_slab->right = NULL;
    new_slab->up = NULL;
    new_slab->type = 0;
    new_slab->x = 0;
    new_slab->y = 0;
    new_slab->affichage = NULL;

    return new_slab;
}

Terrain* _initialize_new_terrain(){
     Terrain* t = malloc(sizeof( Terrain)); // Création du terrain
    if (t == NULL){
        exit(0);
    }
    t->initial_slab = _initiate_new_slab();
    t->spawn_slab = NULL;
    t->ghost_house = NULL;
    t->size_x = 0;
    t->size_y = 0;
    return t;
}

void _load_terrain( Terrain* t){ // Fonction principale d'initialisation du terrain

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

    Slab* current_slab = t->initial_slab;    // Initialisation de l'algorithme de génération
    char c = fgetc(f);
    int x = 0, y = 0;

    printf("Variables correctement initialisées\n");

    while (c != EOF){
        
        if (c == '\n'){ // Cas de retour à la ligne dans le fichier
            current_slab = move_straight(t->initial_slab, DOWN, FULL);
            y += 1;
            x = 0;
        }else{
            Slab* new_slab = _initiate_new_slab();
            
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

    t->initial_slab = t->initial_slab->down;
    free(t->initial_slab->up);

    current_slab = move_straight(t->initial_slab, RIGHT, FULL);
    current_slab = move_straight(current_slab, DOWN, FULL);
    t->size_x = current_slab->x + 1;
    t->size_y = current_slab->y + 1;
    printf("Terrain de %d par %d\n", t->size_x, t->size_y);

    printf("Chargement terminé\n");

    if (!has_spawn){
        printf("Warning: No spawn defined");
    }
    if (!has_ghost_house){
        printf("Warning: No ghost house defined");
    }

}

struct slab* fieldBrowsing(struct slab* slab, int dir, unsigned int n){
    /* Returns the nth slabs in the direction dir, starting from "slab".
    * Trying to return an non-existent slab will result in 
    * fieldBrowsing returning the last valid slab */
    if (n==0){return slab;}
    switch (dir){
        case (UP):
            if (slab->up == NULL) {return slab;}
            return (fieldBrowsing(slab->up, UP, n-1));
        case (RIGHT):
            if (slab->right == NULL) {return slab;}
            return (fieldBrowsing(slab->right, RIGHT, n-1));
        case (DOWN):
            if (slab->down == NULL) {return slab;}
            return (fieldBrowsing(slab->down, DOWN, n-1));
        default:
            if (slab->left == NULL) {return slab;}
            return (fieldBrowsing(slab->left, LEFT, n-1));
    }
}

void _set_warp(Terrain* t){   // Lie les tunnels aux extrémités 
    
    printf("Setting warps:\n");
    
    Slab* horizontal_warp_1 = t->initial_slab;
    Slab* vertical_warp_1 = t->initial_slab;
    
    while (horizontal_warp_1 != NULL){
        if (horizontal_warp_1->type == 1){
            
            // Lien des warpzones verticalement
            Slab* horizontal_warp_2 = move_straight(horizontal_warp_1, RIGHT, FULL);
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
            Slab* vertical_warp_2 = move_straight(vertical_warp_1, DOWN, FULL);
            vertical_warp_2->type = 1;
            vertical_warp_1->left = vertical_warp_2;
            vertical_warp_2->right = vertical_warp_1;
            printf("Warp horizontal créé entre (%d, %d) et (%d, %d)\n", vertical_warp_1->x, vertical_warp_1->y, vertical_warp_2->x, vertical_warp_2->y);

        }

        vertical_warp_1 = vertical_warp_1->down;

    }

    printf("Warps setting done\n");

}

void describe_slab(Slab* current_slab){
    printf("Je suis (%d, %d)\n", current_slab->x, current_slab->y);
    if (current_slab->up != NULL){
        printf("Au dessus, il y a (%d, %d)\n", current_slab->up->x, current_slab->up->y);
    }else{
        printf("Pas de voisin au dessus\n");
    }
    if (current_slab->down != NULL){
        printf("Au dessous, il y a (%d, %d)\n", current_slab->down->x, current_slab->down->y);
    }else{
        printf("Pas de voisin au dessous\n");
    }
    if (current_slab->left != NULL){
        printf("A gauche, il y a (%d, %d)\n", current_slab->left->x, current_slab->left->y);
    }else{
        printf("Pas de voisin a gauche\n");
    }
    if (current_slab->right != NULL){
        printf("A droite, il y a (%d, %d)\n", current_slab->right->x, current_slab->right->y);
    }else{
        printf("Pas de voisin a droite\n");
    }
}

 Terrain* initiate_terrain(){
    printf("Démarrage\n");
    printf("Initialisation\n");

     Terrain* t = _initialize_new_terrain();   // Initializing terrain
    _load_terrain(t);
    _set_warp(t);

    /*int i = 0;
     slab* current_slab = t->initial_slab;
    
    while (i != 5){ // Exploration du terrain pour tester les différentes ures
        scanf("%d", &i);
        current_slab = _move(current_slab, i);
    };*/
    
    printf("Done\n");
    
    return t;
}

int slab_Is_Path(struct slab* slab){
    /* Returns 1 if slab is a path, with or without candy;
    * returns 0 otherwise */
    int type = slab->type;
    if ((type == WALL) || (type == SPAWN) || (type == GHOST_HOUSE)){return 0;}
    return 1;
}