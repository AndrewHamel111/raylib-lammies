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
#include "card/resources.h"
#include "object/management.h"
#include "cursor.h"

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
	if (IsKeyPressed(KEY_F1))
	{
		GameCleanup();
//		ReturnToMainMenu();
	}

	float ft = GetFrameTime();

	ObjectsTick(ft);
    CardManagerUpdate(ft);
	TickCardLocks(ft);

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
		// TODO: god I can't wait for cards to be objects this sh is so annoying
		ObjectsDraw();
        CardManagerDrawAllCards();

		if (DebugShowDeckDrawTest())
		{
			float offset = RESERVE_STACK_OFFSET;
			Vector2 sz = CARD_SIZE;
			Rectangle dest = R(256, 128, sz.x, sz.y);
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);
			dest.y -= offset;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);
			dest.y -= offset;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);
			dest.y -= offset;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);

			dest.y += 300;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);
			dest.y -= offset;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);
			dest.y -= offset;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);

			dest.y += 300;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);
			dest.y -= offset;
			DrawTexturePro(GetCardBackLarge(), GetCardSourceLarge(), dest, V(0,0), 0.0f, WHITE);

			float soffset = RESERVE_STACK_OFFSET;
			Vector2 szSm = CARD_SIZE_SMALL;
			Rectangle sdest = R(512, 128, szSm.x, szSm.y);
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
			sdest.y -= soffset;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
			sdest.y -= soffset;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
			sdest.y -= soffset;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);

			sdest.y += 200;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
			sdest.y -= soffset;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
			sdest.y -= soffset;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);

			sdest.y += 200;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
			sdest.y -= soffset;
			DrawTexturePro(GetCardBackSmall(), GetCardSourceSmall(), sdest, V(0,0), 0.0f, WHITE);
		}

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
