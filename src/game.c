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

		if (DebugShowDeckDrawTest())
		{
			float offset = 6;
			Rectangle dest = R(256, 128, 168, 240);
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

			float soffset = 6;
			Rectangle sdest = R(512, 128, 80, 116);
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
        DrawFPS(10, 10);
	}
	EndDrawing();
}

void GameCleanup(void)
{
    // game cleanup //
}