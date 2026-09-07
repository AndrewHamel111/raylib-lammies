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
	Vector2 mpos = GetMousePosition();

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
        DrawFPS(10, 10);
	}
	EndDrawing();
}

void GameCleanup(void)
{
    // game cleanup //
}