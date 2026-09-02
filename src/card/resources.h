#pragma once

#include "card.h"

Texture2D GetCardSmall(const Card* card);
Texture2D GetCardLarge(const Card* card);

Texture2D GetCardBackSmall(void);
Texture2D GetCardBackLarge(void);

Texture2D GetCardJokerSmall(bool red);
Texture2D GetCardJokerLarge(bool red);

Rectangle GetCardSourceSmall(void);
Rectangle GetCardSourceLarge(void);

Texture2D GetCardValue(Suit suit, Rank rank, bool small);
