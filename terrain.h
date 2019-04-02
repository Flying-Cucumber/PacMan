#ifndef __TERRAIN_H__
#define __TERRAIN_H__

struct case{
    struct case * up
    struct case * down
    struct case * left
    struct case * right
    int _type
    int _x
    int _y
    int pac_gum
}