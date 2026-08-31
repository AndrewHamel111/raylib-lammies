#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "deck.h"

static uint deck[52] = {0};
static int deck_count = 0;

void InitDeck(void)
{
	for (int i = 0; i < 52; i++)
	{
		deck[i] = 51 - i;
	}
	deck_count = 52;
}

Card DrawNewCard(void)
{
	uint value = DrawNewCardValue();

	Card result;
	result.suit = value / 13;
	result.rank = value % 13;
	return result;
}

uint DrawNewCardValue(void)
{
	uint top = deck[deck_count - 1];
	deck[deck_count - 1] = -1;
	deck_count--;

	return top;
}

void ReturnCard(Card card)
{
	ReturnCardTo(card, DeckAnywhere);
}

// UNTESTED
void ReturnCardTo(Card card, DeckLocation where)
{
	uint cardValue = CardAsInt(card);
	switch (where)
	{
		case DeckTop:
			deck[deck_count] = cardValue;
			break;
		case DeckMiddle:
			// special cases for such a low number of cards
			if (deck_count == 1)
			{
				if (GetRandomValue(0, 1))
				{
					deck[1] = cardValue;
				}
				else
				{
					deck[1] = deck[0];
					deck[0] = cardValue;
				}
			}
			else if (deck_count == 2)
			{
				deck[2] = deck[1];
				deck[1] = cardValue;
			}
			else if (deck_count < 8)
			{
				int idx = deck_count / 2;
				memmove(deck + idx + 1, deck + idx, (deck_count - idx) * sizeof(uint));
				deck[idx] = cardValue;
			}
			else if (deck_count < 25)
			{
				int idx = GetRandomValue(4, deck_count - 4 - 1);
				memmove(deck + idx + 1, deck + idx, (deck_count - idx) * sizeof(uint));
				deck[idx] = cardValue;
			}
			break;
		case DeckBottom:
			memmove(deck + 1, deck, deck_count * sizeof(uint));
			deck[0] = cardValue;
			break;
		default:
			TraceLog(LOG_WARNING, "ReturnCardTo: Invalid DeckLocation, choosing Anywhere");
		case DeckAnywhere:
		{
			int index = GetRandomValue(0, deck_count);
			if (index == deck_count)
			{
				deck[deck_count] = cardValue;
			}
			else
			{
				memmove(deck + index + 1, deck + index, (deck_count - index) * sizeof(uint));
				deck[index] = cardValue;
			}
			break;
		}
	}

	deck_count++;
}

void Shuffle(void)
{
	if (deck_count > 1)
	{
		size_t i;
		for (i = 0; i < deck_count - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (deck_count - i) + 1);
			uint t = deck[j];
			deck[j] = deck[i];
			deck[i] = t;
		}
	}
}

int GetDeckCount(void)
{
	return deck_count;
}
