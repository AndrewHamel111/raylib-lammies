#include "cursor.h"
#include "resources.h"
#include "utility.h"
#include "constants.h"

static bool cursor_enabled = false;
static CursorState cursor_state = CursorDefault;
static CursorHeight cursor_height = CursorHeightTop;

void CursorDraw(void)
{
	if (!cursor_enabled) return;

	Vector2 pos = CursorGetPos();
	Texture2D tex = GetCursorTex(cursor_state);
	Rectangle dest = R(pos.x, pos.y, CURSOR_SIZE, CURSOR_SIZE);

	DrawTexturePro(tex, R(0,0,tex.width,tex.height), dest, V(dest.width * 0.2f,dest.height * 0.1f), 0, WHITE);
}

void CursorSetState(CursorState state)
{
	if (cursor_state == state) return;

	cursor_state = state;
}

void CursorSetHeight(CursorHeight height)
{
	if (cursor_height == height) return;

	cursor_height = height;
}

CursorHeight CursorHeightGet(void)
{
	return cursor_height;
}

Vector2 CursorGetPos(void)
{
	return GetMousePosition();
}

void CursorSetEnabled(bool enabled)
{
	cursor_enabled = enabled;

	if (cursor_enabled)
	{
		HideCursor();
	}
	else
	{
		ShowCursor();
	}
}
