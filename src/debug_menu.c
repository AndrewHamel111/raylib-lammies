#include "debug_menu.h"
#include "debug.h"
#include "raylib.h"
#include "external/raygui.h"

#include "utility.h"
#include "constants.h"
#include "resources.h"

//#define MENU_WIDTH 200
static int MENU_WIDTH = 200;
static int PADDING = 10;

// Add extern functions here for important gameplay logic you want to trigger! Alternatively, add a button which sets a
// bool here, and poll that bool from the appropriate place in the game's logic

static bool showDebugMenu;
static bool showDrawTests;
static bool drawDeckTest;
static bool drawObjectHitboxes;
static bool drawCardsSmall;
static bool showObjectStack;
static bool useVSync;

// This style of bool (disposed after use) is an annoying enough pattern (static bool, bool function, prototype in
// debug.h) it may be possible to have a macro which defines all 3 parts.
static bool spawnFactoryDeck;
static bool spawnShuffledDeck;
static bool clearCards;
static bool cleanupObjects;

// NOT RECOMMENDED: values here are not static so they can be extern'd from gameplay code. AVOID IF POSSIBLE

// Define debug.h functions here

bool DebugExampleBool(void)
{
	return showDrawTests;
}

bool DebugShowDeckDrawTest(void)
{
	return drawDeckTest;
}

bool DebugDrawObjectHitboxes(void)
{
	return drawObjectHitboxes;
}

bool DebugDrawCardsSmall(void)
{
	return drawCardsSmall;
}

bool DebugShowObjectStack(void)
{
	return showObjectStack;
}

bool DebugSpawnFactoryDeck(void)
{
    if (spawnFactoryDeck)
    {
		spawnFactoryDeck = false;
        return true;
    }

    return false;
}

bool DebugSpawnShuffledDeck(void)
{
	if (spawnShuffledDeck)
	{
		spawnShuffledDeck = false;
		return true;
	}

	return false;
}

bool DebugClearCards(void)
{
	if (clearCards)
	{
		clearCards = false;
		return true;
	}

	return false;
}

bool DebugCleanupObjects(void)
{
	if (cleanupObjects)
	{
		cleanupObjects = false;
		return true;
	}

	return false;
}

// Debug menu drawing logic

static float X;
static float Y;

static float X_start;
static float Y_start;

static bool right_half = false;

static Rectangle NextCheckboxRec(void)
{
	Rectangle r = R(X, Y, (2 * PADDING), (2 * PADDING));

	Y += (2 * PADDING) + (PADDING);

	return r;
}

static Rectangle NextHalfButton(void)
{
	Rectangle r = R(X, Y, (MENU_WIDTH * 0.5f) - (PADDING), (2 * PADDING));

	if (right_half)
	{
		X = X_start + (PADDING);
		Y += (2 * PADDING) + (PADDING);
	}
	else
	{
		X += (MENU_WIDTH * 0.5f) + (PADDING);
	}

	right_half = !right_half;

	return r;
}

static Rectangle NextButton(void)
{
	Rectangle r = R(X, Y, MENU_WIDTH, (2 * PADDING));

	Y += (2 * PADDING) + (PADDING);

	return r;
}

static Rectangle NextLabel(void)
{
	Rectangle r = R(X, Y, MENU_WIDTH, (PADDING));

	Y += (PADDING) + (PADDING);

	return r;
}

void DebugMenuDraw(void)
{
	if (IsKeyPressed(KEY_GRAVE))
	{
		showDebugMenu = !showDebugMenu;
		Vector2 mpos = GetMousePosition();
		X_start = mpos.x;
		Y_start = mpos.y;
	}

	if (!showDebugMenu)
	{
		return;
	}

	X = X_start + (PADDING);
	Y = Y_start + (PADDING);

	GuiLabel(NextLabel(), "-== Debug Vis ==-");
	GuiCheckBox(NextCheckboxRec(), "Draw Object Hitboxes", &drawObjectHitboxes);
	GuiCheckBox(NextCheckboxRec(), "Show Object Stack", &showObjectStack);
	GuiLabel(NextLabel(), "-== Spawn Objects ==-");
    if(GuiButton(NextHalfButton(), "Fresh Deck"))
    {
		spawnFactoryDeck = true;
    }
	if(GuiButton(NextHalfButton(), "Shuffled Deck"))
	{
		spawnShuffledDeck = true;
	}
	GuiLabel(NextLabel(), "-== Object Management ==-");
	if(GuiButton(NextHalfButton(), "Cleanup Cards"))
	{
		clearCards = true;
	}
	if(GuiButton(NextHalfButton(), "Cleanup Objects"))
	{
		cleanupObjects = true;
	}

	GuiLabel(NextLabel(), "-== Settings ==-");
	if (GuiCheckBox(NextCheckboxRec(), "Use VSync", &useVSync))
	{
		if (useVSync)
		{
			SetWindowState(FLAG_VSYNC_HINT);
            SetTargetFPS(-1);
		}
		else
		{
			ClearWindowState(FLAG_VSYNC_HINT);
            SetTargetFPS(TARGET_FPS);
		}
	}
	if (GuiCheckBox(NextCheckboxRec(), "Use Small Cards", &drawCardsSmall))
	{

	}
}

void DebugMenuSetScaling(int scale)
{
	if (scale <= 1)
	{
		GuiSetFont(GetFontDefault());
		GuiSetStyle(DEFAULT, TEXT_SIZE, 10);
		GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
		MENU_WIDTH = 200;
		PADDING = 10;
		return;
	}

	GuiSetFont(GetMonoFont());
	GuiSetStyle(DEFAULT, TEXT_SIZE, 10 * scale);
	GuiSetStyle(DEFAULT, TEXT_SPACING, 0.5f * scale);
	MENU_WIDTH = 200 * (scale);
	PADDING = 10 + (10 * (scale * 0.25f));
}
