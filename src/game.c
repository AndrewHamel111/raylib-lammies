#include "game.h"
#include "raylib.h"
#include "raymath.h"

#include "resources.h"

#include "utility.h"
#include "constants.h"

#include "debug.h"
#include "debug_menu.h"

#include "utility/lock_timers.h"
#include "object/card/resources.h"
#include "object/management.h"
#include "cursor.h"

#include <string.h>
#include <stdlib.h>

//extern void ReturnToMainMenu();

void GameInit(void)
{
	// game setup //
	DebugMenuSetScaling(3);
}

void GameLoop(void)
{
    // Update //
	if (IsKeyPressed(KEY_F1))
	{
		GameCleanup();
//		ReturnToMainMenu();
	}

	float ft = GetFrameTime();

	ObjectsTick(ft);
	TickObjectLocks(ft);

    // Draw //
	BeginDrawing();
	{
		ClearBackground(RAYWHITE);

		if (CursorHeightGet() == CursorHeightTable)
		{
			CursorDraw();
		}

		// TODO: some feedback that an object is hovering too close to another object would be nice, it's hard to tell where the valid placement area is.
		// TODO: perhaps a visualization of all object hitboxes when moving objects, so it's clear where you're not allowed to place an object. In that case, should we be inflate the OTHER object's hitboxes instead
		ObjectsDraw();

        DebugMenuDraw();
		if (CursorHeightGet() == CursorHeightTop)
		{
			CursorDraw();
		}

		if (DebugShowObjectStack())
		{
			float fontSize = 32;
			int spacer = 4;
			int objectsCount;
			Object** objects = ObjectsGetOrdered(&objectsCount);
			DrawRectangleRec(R(0, 0, 320, (fontSize + spacer) * (objectsCount + 4)), Fade(BLACK, 0.75f));
			for (int i = 0; i < objectsCount + 4; i++)
			{
				Color color = i < objectsCount ? WHITE : GRAY;
				int id = objects[i] ? objects[i]->id : 0;
				DrawTextEx(GetMonoFont(), TextFormat("[%d]: #%d", i, id), V(spacer, spacer + (i * (fontSize + spacer))), fontSize, 1.0f, color);
			}
		}
//        DrawFPS(10, 10);
	}
	EndDrawing();
}

void GameCleanup(void)
{
    // game cleanup //
}
