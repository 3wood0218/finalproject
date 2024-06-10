#include "gamescene.h"
#include "../element/enemy1.h"
#include<stdlib.h>
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<time.h>
#include <stdbool.h>

/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/stage.jpg");
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    pObj->pDerivedObj = pDerivedObj;
    // register elemen
    _Register_elements(pObj, New_Floor(Floor_L));
    _Register_elements(pObj, New_Teleport(Teleport_L));
    srand(time(NULL));
    int x = (rand()%900);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    _Register_elements(pObj, New_enemy(Enemy_L,x));
    _Register_elements(pObj, New_Character(Character_L));
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}
unsigned int clk=500;
static char str_health[12]; 
static char str_score[20];// 確保有足夠的空間來存儲整數和終止符
void game_scene_update(Scene *self)
{   
    clk-=1;
    if(!pause)
    {
    t_clock++;
    }
    if(pause){
        if(key_state[ALLEGRO_KEY_P]&&(clk<-20))
            {
            pause=false;
            }
        if(key_state[ALLEGRO_KEY_M]){
            self->scene_end=true;
            pause=false;
            t_clock=0;
            score=0;
            health=1000;
            stage=0;
            window=0;
        }
        if(key_state[ALLEGRO_KEY_R]){
            self->scene_end=true;
            pause=false;
            score=0;
            t_clock=0;
            stage=0;
            health=1000;
            window=1;
        }
        return;
    }
    if(key_state[ALLEGRO_KEY_P]&&clk>40){
        pause=true;
    }
    if(health<=0){
            self->scene_end=true;
            window=2;
    }
    if(t_clock>=4000){
        self->scene_end=true;
        t_clock=0;
        window=3;
    }
    int randx = (rand()%800);
    if((clk%350==0)){
    Elements *ene = New_enemy(Enemy_L,randx);
    _Register_elements(self,ene);
    randx=rand()%800;
    if(t_clock>=500&&stage>=1)
    {
        Elements *ene2 = New_enemy2(Enemy2_L,randx);
        _Register_elements(self,ene2);
        randx=(rand()%800);
    }
    if(t_clock>=1500&&stage>=2)
    {
        Elements *ene3 = New_enemy3(Enemy3_L,randx);
        _Register_elements(self,ene3);
        randx=(rand()%800);
    }
    if(t_clock>=2500&&stage>=3){
        Elements *ene4 = New_enemy4(Enemy4_L,randx);
        _Register_elements(self,ene4);
    }
        if(clk<=0){
            clk=500;
        }
    }
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        allEle.arr[i]->Update(allEle.arr[i]);
    }
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        // run every interact object
        for (int j = 0; j < ele->inter_len; j++)
        {
            int inter_label = ele->inter_obj[j];
            ElementVec labelEle = _Get_label_elements(self, inter_label);
            for (int i = 0; i < labelEle.len; i++)
            {
                ele->Interact(ele, labelEle.arr[i]);
            }
        }
    }
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
    
}
void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
    if(health>=0)
    {
        sprintf(str_health, "%d", health);
        const char *healthstr = str_health;
        al_draw_rectangle(50,640,70+(health/3), 645, al_map_rgb(255,0,0),10);
        al_draw_text(gs->font, al_map_rgb(255, 255, 255), 45,635, ALLEGRO_ALIGN_LEFT, healthstr);    
    }
    if(pause){
        al_draw_rectangle(275,100,375,550,al_map_rgb(255,255,255),100);
        al_draw_rectangle(525,100,625,550,al_map_rgb(255,255,255),100);
        al_draw_text(gs->font, al_map_rgb(255, 255, 255), 10,10, ALLEGRO_ALIGN_LEFT, "Press P to resume");
        al_draw_text(gs->font, al_map_rgb(255, 255, 255), 10,30, ALLEGRO_ALIGN_LEFT, "Press R to Restart");  
        al_draw_text(gs->font, al_map_rgb(255, 255, 255), 10,50, ALLEGRO_ALIGN_LEFT, "Press M back to menu"); 
        }
    if(!pause){
    sprintf(str_score,"%d",score);
    const char *scorestr=str_score;
    al_draw_text(gs->font,al_map_rgb(255,255,255),10,10,ALLEGRO_ALIGN_LEFT,scorestr);
    }
}
void game_scene_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
