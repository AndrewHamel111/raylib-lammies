#pragma once

#include "card.h"

typedef enum DeckLocation
{
	DeckTop = 0,
	DeckMiddle,
	DeckBottom,
	DeckAnywhere
} DeckLocation;

void InitDeck(void);

Card DrawNewCard(void);
unsigned int DrawNewCardValue(void);
// Shorthand for returning Anywhere
void ReturnCard(Card card);
void ReturnCardTo(Card card, DeckLocation where);
void Shuffle(void);

int GetDeckCount(void);

