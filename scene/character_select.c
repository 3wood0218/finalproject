#include "character_select.h"
#include "global.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

typedef struct CharacterSelect {
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *background;
    int selected_character;
} CharacterSelect;

Scene *New_Character_Select(int label)
{
    CharacterSelect *pDerivedObj = (CharacterSelect *)malloc(sizeof(CharacterSelect));
    Scene *pObj = New_Scene(label);
    
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    pDerivedObj->background = al_load_bitmap("assets/image/tutoral.jpg");
    pDerivedObj->selected_character = -1;
    
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = character_select_update;
    pObj->Draw = character_select_draw;
    pObj->Destroy = character_select_destroy;
    
    return pObj;
}

void character_select_update(Scene *self)
{
    CharacterSelect *Obj = ((CharacterSelect *)(self->pDerivedObj));

    if (key_state[ALLEGRO_KEY_1]) {
        Obj->selected_character = 0;  // Lee Sin
    } else if (key_state[ALLEGRO_KEY_2]) {
        Obj->selected_character = 1;  // Ezreal
    } else if (key_state[ALLEGRO_KEY_3]) {
        Obj->selected_character = 2;  // Taliyah
    }
    
    if (Obj->selected_character != -1 && key_state[ALLEGRO_KEY_ENTER]) {
        selected_character = Obj->selected_character;
        stage = GAME;
        window = 2;  // 设置为游戏场景
        self->scene_end = true;
    }
}

void character_select_draw(Scene *self)
{
    CharacterSelect *Obj = ((CharacterSelect *)(self->pDerivedObj));
    al_draw_bitmap(Obj->background, 0, 0, 0);
    
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTRE, "Select Your Character");
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "1. Lee Sin");
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 20, ALLEGRO_ALIGN_CENTRE, "2. Ezreal");
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 40, ALLEGRO_ALIGN_CENTRE, "3. Taliyah");
}

void character_select_destroy(Scene *self)
{
    CharacterSelect *Obj = ((CharacterSelect *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_bitmap(Obj->background);
    free(Obj);
    free(self);
}
