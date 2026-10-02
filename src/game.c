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
#include "input.h"

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

	InputUpdate();
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

		ObjectActionList actions = GetCurrentActionList();
//		const int boxWidth = 800;
		const int lineHeight = 60;
		const int linePad = 10;
		const int actionFontSize = lineHeight - linePad;
		if (actions.count > 0)
		{
			const char* textLines[4]; // see MAX_TEXTFORMAT_BUFFERS in rtext.c
			textLines[0] = TextFormat("1 - %s", ObjectActionName(actions.actions[0]));
			textLines[1] = TextFormat("2 - %s", ObjectActionName(actions.actions[1]));
			textLines[2] = TextFormat("3 - %s", ObjectActionName(actions.actions[2]));
			textLines[3] = TextFormat("4 - %s", ObjectActionName(actions.actions[3]));

			int boxWidth = 75 + (MAX(MeasureText(textLines[0], actionFontSize),
								 MAX(MeasureText(textLines[1], actionFontSize),
								 MAX(MeasureText(textLines[2], actionFontSize),
								 MeasureText(textLines[3], actionFontSize)))));

			int boxHeight = lineHeight * actions.count + linePad;
			DrawRectangle(0, GetScreenHeight() - boxHeight, boxWidth, boxHeight, Fade(BLACK, 0.3f));

			for (int i = actions.count - 1; i >= 0; i--)
			{
				int textY = GetScreenHeight() - (lineHeight * (actions.count - i));
				const char* text = TextFormat("%d - %s", i + 1, ObjectActionName(actions.actions[i]));
				DrawText(text, 50, textY, lineHeight - linePad, BLACK);
			}
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
