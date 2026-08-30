#include "game.h"
#include "raylib.h"
#include "raymath.h"

#include "resources.h"

#include "utility.h"
#include "constants.h"

#include "debug.h"
#include "debug_menu.h"

#include <string.h>
#include <stdlib.h>

//extern void ReturnToMainMenu();

void GameInit(void)
{
	// game setup //
}

void GameLoop(void)
{
    // Update //
	Vector2 mpos = GetMousePosition();

	if (IsKeyPressed(KEY_F1))
	{
		GameCleanup();
//		ReturnToMainMenu();
	}

    // Draw //
	BeginDrawing();
	{
		ClearBackground(RAYWHITE);
	}
	EndDrawing();
}

void GameCleanup(void)
{
    // game cleanup //
}