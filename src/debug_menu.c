#include "debug_menu.h"
#include "debug.h"
#include "raylib.h"
#include "external/raygui.h"

#include "utility.h"
#include "constants.h"

#define MENU_WIDTH 300

// Add extern functions here for important gameplay logic you want to trigger! Alternatively, add a button which sets a
// bool here, and poll that bool from the appropriate place in the game's logic

static bool showDebugMenu;
static bool exampleBool;
static bool useVSync;

// This style of bool (disposed after use) is an annoying enough pattern (static bool, bool function, prototype in
// debug.h) it may be possible to have a macro which defines all 3 parts.
static bool spawnCard;
static bool deckTest;
static bool shuffleDeck;
static bool clearCards;
static bool reinitDeck;

// NOT RECOMMENDED: values here are not static so they can be extern'd from gameplay code. AVOID IF POSSIBLE
bool debugDrawCardsSmall;

// Define debug.h functions here

bool DebugExampleBool(void)
{
	return exampleBool;
}

bool DebugSpawnCard(void)
{
    if (spawnCard)
    {
        spawnCard = false;
        return true;
    }

    return false;
}

bool DebugDeckTest(void)
{
	if (deckTest)
	{
		deckTest = false;
		return true;
	}

	return false;
}

bool DebugShuffleDeck(void)
{
	if (shuffleDeck)
	{
		shuffleDeck = false;
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

bool DebugReinitDeck(void)
{
	if (reinitDeck)
	{
		reinitDeck = false;
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
	Rectangle r = R(X, Y, 20, 20);

	Y += 20 + 10;

	return r;
}

static Rectangle NextHalfButton(void)
{
	Rectangle r = R(X, Y, 50, 20);

	if (right_half)
	{
		X = X_start + 10;
		Y += 20 + 10;
	}
	else
	{
		X += 50 + 10;
	}

	right_half = !right_half;

	return r;
}

static Rectangle NextButton(void)
{
	Rectangle r = R(X, Y, 110, 20);

	Y += 20 + 10;

	return r;
}

static Rectangle NextLabel(void)
{
	Rectangle r = R(X, Y, MENU_WIDTH, 10);

	Y += 10 + 10;

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

	X = X_start + 10;
	Y = Y_start + 10;

	GuiCheckBox(NextCheckboxRec(), "Example Bool", &exampleBool);
	if (exampleBool)
	{
		GuiLabel(NextLabel(), "Extra Controls");
		if (GuiButton(NextHalfButton(), "<--"))
		{
		}
		if (GuiButton(NextHalfButton(), "-->"))
		{
		}
	}
    if(GuiButton(NextButton(), "Spawn Card"))
    {
        spawnCard = true;
    }
	if(GuiButton(NextButton(), "Deck Test"))
	{
		deckTest = true;
	}
	if(GuiButton(NextButton(), "Shuffle Deck"))
	{
		shuffleDeck = true;
	}
	if(GuiButton(NextButton(), "Clear Cards"))
	{
		clearCards = true;
	}
	if(GuiButton(NextButton(), "Reinit Deck"))
	{
		reinitDeck = true;
	}

	GuiLabel(NextButton(), "-== Settings ==-");
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
	if (GuiCheckBox(NextCheckboxRec(), "Use Small Cards", &debugDrawCardsSmall))
	{

	}
}
