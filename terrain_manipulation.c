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

struct slab* initiate_new_slab(){
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
            printf("Coordonnées: (%d, %d)\n", x, y);
            if (new_slab->left != NULL){
                printf("Liée par la gauche à (%d, %d)\n", new_slab->left->x, new_slab->left->y);
            }
            if (new_slab->up != NULL){
                printf("Liée par le haut à (%d, %d)\n", new_slab->up->x, new_slab->up->y);
            }
            new_slab->type = (int) c;
            current_slab = new_slab;
            x += 1;
        }
        c = fgetc(f);
    }
    t->spawn_slab = current_slab;
    printf("current_slab: (%d, %d)", t->spawn_slab->x, t->spawn_slab->y);
    printf("Chargement terminé\n");
}

void move(struct terrain* t, int direction){
    struct slab* current_slab = t->spawn_slab;
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
            if (current_slab->left != NULL){
                current_slab = current_slab->left;
            }
            break;
        default:
            break;
    }
    printf("(%d, %d)\n", current_slab->x, current_slab->y);
}

int main(){
    printf("Démarrage\n");
    printf("Initialisation\n");
    int i = 0;
    struct terrain* t = initialize_new_terrain();
    load_terrain(t);
    while (i != 5){
        scanf("%d", &i);
        move(t, i);
    };
    printf("done\n");
    return 0;
}