#pragma once

#include <stdbool.h>
#include "raylib.h"

typedef enum CursorState
{
	CursorDefault = 0,
	CursorPick,
	CursorHold,
	CursorPalm,
} CursorState;

typedef enum CursorHeight
{
	CursorHeightTable,
	CursorHeightTop,
} CursorHeight;

void CursorDraw(void);
void CursorSetState(CursorState state);
CursorHeight CursorHeightGet(void);
void CursorSetHeight(CursorHeight height);

Vector2 CursorGetPos(void);

void CursorSetEnabled(bool enabled);
