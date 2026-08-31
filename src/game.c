#include "game.h"
#include "raylib.h"
#include "raymath.h"

#include "resources.h"

#include "utility.h"
#include "constants.h"

#include "debug.h"
#include "debug_menu.h"

#include "card.h"
#include "card/manager.h"
#include "card/lock_timers.h"

#include <string.h>
#include <stdlib.h>

//extern void ReturnToMainMenu();

void GameInit(void)
{
	// game setup //
    CardManagerInit();
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

	float ft = GetFrameTime();
    CardManagerUpdate(ft);
	TickCardLocks(ft);

    // Draw //
	BeginDrawing();
	{
		ClearBackground(RAYWHITE);

        CardManagerDrawAllCards();

        DebugMenuDraw();
        DrawFPS(10, 10);
	}
	EndDrawing();
}

void GameCleanup(void)
{
    // game cleanup //
}