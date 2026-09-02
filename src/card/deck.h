#pragma once

#include "card.h"
#include <sys/types.h>
#include "constants.h"

typedef enum DeckLocation
{
	DeckTop = 0,
	DeckMiddle,
	DeckBottom,
	DeckAnywhere
} DeckLocation;

typedef struct Deck
{
	uint arr[DECK_MAX];
	int count;
} Deck;

void InitDeck(Deck* deck);

Card DrawNewCard(Deck* deck);
uint DrawNewCardValue(Deck* deck);
void ReturnCard(Deck* deck, Card card);
void ReturnCardTo(Deck* deck, Card card, DeckLocation where);
void Shuffle(Deck* deck);

int GetDeckCount(const Deck* deck);

