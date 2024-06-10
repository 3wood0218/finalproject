#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "over.h"
#include "win.h"
Scene *scene = NULL;
void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L);
        break;
    case Over_L:
        scene = New_Over(Over_L);
        break;
    case WIN_L:
        scene=New_winner(WIN_L);
    default:
        break;
    }
}