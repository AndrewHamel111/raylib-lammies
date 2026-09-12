#pragma once

#include <stdbool.h>
#include "utility/types.h"
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

uint DrawNewCardValue(Deck* deck);
void ReturnCard(Deck* deck, int value);
void ReturnCardTo(Deck* deck, int value, DeckLocation where);
void Shuffle(Deck* deck);

int GetDeckCount(const Deck* deck);
int GetDeckMax(const Deck* deck);
bool DeckIsFull(const Deck* deck);
