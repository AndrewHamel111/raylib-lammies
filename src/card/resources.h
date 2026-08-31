#pragma once

#include "card.h"

Texture2D GetCardSmall(const Card* card);
Texture2D GetCardLarge(const Card* card);

Rectangle GetCardSourceSmall(void);
Rectangle GetCardSourceLarge(void);

Texture2D GetCardValue(Suit suit, Rank rank, bool small);
