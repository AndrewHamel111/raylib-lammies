#include "menu.h"

#include "external/raygui.h"

void MenuInit(void)
{

}

void MenuLoop(void)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawText("Menu not implemented", 10, 10, 32, BLACK);
    }
    EndDrawing();
}
