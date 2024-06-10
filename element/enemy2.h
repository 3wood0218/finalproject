#ifndef enemy2_H_INCLUDED
#define enemy2_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [enemy2 object]
*/
typedef struct _enemy2
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
    int blood;
} enemy2;
Elements *New_enemy2(int label,int x);
void enemy2_update(Elements *self);
void enemy2_interact(Elements *self, Elements *tar);
void enemy2_draw(Elements *self);
void enemy2_destory(Elements *self);
void _enemy2_update_position(Elements *self, int dx, int dy);

#endif
