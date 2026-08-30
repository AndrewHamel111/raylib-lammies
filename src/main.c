#include "raylib.h"
#include "raymath.h"
	
#include <math.h>
#include <string.h>

#include "constants.h"
#include "game.h"
#include "menu.h"
#include "timer.h"

#include "resources.h"

bool showGame = false;
bool quitApp = false;

void StartMusic(void);

int main(void)
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	InitAudioDevice();

	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText("LOADING...", 100, 100, 200, BLACK);
	EndDrawing();

	LoadResources();

#ifdef SKIP_MENU
    GameInit();
    showGame = true;
#else
	MenuInit();
#endif

	SetTimer(0.5f, StartMusic);

	while (!WindowShouldClose() && !quitApp)
	{
		if (IsMusicStreamPlaying(GetBGM()))
		{
			UpdateMusicStream(GetBGM());
		}

		bool altPressed = IsKeyPressed(KEY_LEFT_ALT) || IsKeyPressed(KEY_RIGHT_ALT);
		bool altDown = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
		if (altDown && IsKeyPressed(KEY_ENTER) || IsKeyDown(KEY_ENTER) && altPressed)
		{
			ToggleBorderlessWindowed();
		}

		TickTimer(GetFrameTime());
		
		if (showGame)
		{
			GameLoop();
		}
		else
		{
			MenuLoop();
		}
	}

	CloseWindow();

	return 0;
}

void StartMusic(void)
{
    PlayMusicStream(GetBGM());
}
