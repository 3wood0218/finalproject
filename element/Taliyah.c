#include "charater.h"
#include "../scene/sceneManager.h"
#include "projectile.h"
#include "../shapes/Rectangle.h"
#include "../algif5/src/algif.h"
#include <stdio.h>
#include "../shapes/Rectangle.h"
#include <stdbool.h>
/*
   [Character function]
*/
Elements *New_Character(int label)
{
    Character *Taliyah = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[3][10] = {"stop", "move", "attack"};
    for (int i = 0; i < 3; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/taliyah_%s.gif", state_string[i]);
        Taliyah->gif_status[i] = algif_new_gif(buffer, -1);
    }
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/taliyah.mp3");
    Taliyah->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(Taliyah->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(Taliyah->atk_Sound, al_get_default_mixer());

    // initial the geometric information of character
    Taliyah->width = Taliyah->gif_status[0]->width;
    Taliyah->height = Taliyah->gif_status[0]->height;
    Taliyah->x = 300;
    Taliyah->y = HEIGHT - Taliyah->height - 30;
    Taliyah->hitbox = New_Rectangle(Taliyah->x,
                                        Taliyah->y,
                                        Taliyah->x + Taliyah->width,
                                        Taliyah->y + Taliyah->height);
    Taliyah->dir = false; // true: face to right, false: face to left
    // initial the animation component
    Taliyah->state = STOP;
    Taliyah->new_proj = false;
    pObj->pDerivedObj = Taliyah;
    // setting derived object function
    pObj->Draw = Character_draw;
    pObj->Update = Character_update;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destory;
    return pObj;
}
void Character_update(Elements *self)
{
    // use the idea of finite state machine to deal with different state
    Character *chara = ((Character *)(self->pDerivedObj));
    if (chara->state == STOP)
    {
        if (key_state[ALLEGRO_KEY_SPACE])
        {
            chara->state = ATK;
        }
        else if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            chara->state = MOVE;
        }
        else
        {
            chara->state = STOP;
        }
    }
    else if (chara->state == MOVE)
    {
        if (key_state[ALLEGRO_KEY_SPACE])
        {
            chara->state = ATK;
        }
        else if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            _Character_update_position(self, -7, 0);
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            _Character_update_position(self, 7, 0);
            chara->state = MOVE;
        }
        if (chara->gif_status[chara->state]->done)
            chara->state = STOP;
    }
    else if (chara->state == ATK)
    {
        if (chara->gif_status[chara->state]->done)
        {
            chara->state = STOP;
            chara->new_proj = false;
        }
        if (chara->gif_status[ATK]->display_index == 2 && chara->new_proj == false)
        {
            Elements *pro;
            if (chara->dir)
            {
                pro = New_Projectile(Projectile_L,
                                     chara->x + chara->width - 100,
                                     chara->y + 10,
                                     5);
            }
            else
            {
                pro = New_Projectile(Projectile_L,
                                     chara->x - 50,
                                     chara->y + 10,
                                     -5);
            }
            _Register_elements(scene, pro);
            chara->new_proj = true;
        }
    }
}
void Character_draw(Elements *self)
{
    // with the state, draw corresponding image
    Character *chara = ((Character *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (pause){
        frame=algif_get_bitmap(chara->gif_status[STOP], al_get_time());
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }
    if (frame)
    {
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }
    if (chara->state == ATK && chara->gif_status[chara->state]->display_index == 2 &&!pause)
    {
        al_play_sample_instance(chara->atk_Sound);
    }
}
void Character_destory(Elements *self)
{
    Character *Obj = ((Character *)(self->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    for (int i = 0; i < 3; i++)
        algif_destroy_animation(Obj->gif_status[i]);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Character_interact(Elements *self, Elements *tar) {}
