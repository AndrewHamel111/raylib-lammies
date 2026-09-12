#pragma once

#include "raylib.h"

/// DEPRECATED
Texture2D GetCardSmall(int value);
/// DEPRECATED
Texture2D GetCardLarge(int value);

Texture2D GetCardValue(int value, bool small);
Texture2D GetCardBack(bool small);

Rectangle GetCardSource(bool small);
