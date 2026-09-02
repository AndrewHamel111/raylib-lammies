#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "deck.h"

void InitDeck(Deck* deck)
{
	for (int i = 0; i < 52; i++)
	{
		deck->arr[i] = 51 - i;
	}
	deck->count = 52;
}

Card DrawNewCard(Deck* deck)
{
	uint value = DrawNewCardValue(deck);

	Card result;
	result.suit = value / 13;
	result.rank = value % 13;
	return result;
}

uint DrawNewCardValue(Deck* deck)
{
	uint top = deck->arr[deck->count - 1];
	deck->arr[deck->count - 1] = -1;
	deck->count--;

	return top;
}

void ReturnCard(Deck* deck, Card card)
{
	ReturnCardTo(deck, card, DeckAnywhere);
}

// UNTESTED
void ReturnCardTo(Deck* deck, Card card, DeckLocation where)
{
	uint cardValue = CardAsInt(card);
	switch (where)
	{
		case DeckTop:
			deck->arr[deck->count] = cardValue;
			break;
		case DeckMiddle:
			// special cases for such a low number of cards
			if (deck->count == 1)
			{
				if (GetRandomValue(0, 1))
				{
					deck->arr[1] = cardValue;
				}
				else
				{
					deck->arr[1] = deck->arr[0];
					deck->arr[0] = cardValue;
				}
			}
			else if (deck->count == 2)
			{
				deck->arr[2] = deck->arr[1];
				deck->arr[1] = cardValue;
			}
			else if (deck->count < 8)
			{
				int idx = deck->count / 2;
				memmove(deck->arr + idx + 1, deck->arr + idx, (deck->count - idx) * sizeof(uint));
				deck->arr[idx] = cardValue;
			}
			else if (deck->count < 25)
			{
				int idx = GetRandomValue(4, deck->count - 4 - 1);
				memmove(deck->arr + idx + 1, deck->arr + idx, (deck->count - idx) * sizeof(uint));
				deck->arr[idx] = cardValue;
			}
			break;
		case DeckBottom:
			memmove(deck->arr + 1, deck->arr, deck->count * sizeof(uint));
			deck->arr[0] = cardValue;
			break;
		default:
			TraceLog(LOG_WARNING, "ReturnCardTo: Invalid DeckLocation, choosing Anywhere");
		case DeckAnywhere:
		{
			int index = GetRandomValue(0, deck->count);
			if (index == deck->count)
			{
				deck->arr[deck->count] = cardValue;
			}
			else
			{
				memmove(deck->arr + index + 1, deck->arr + index, (deck->count - index) * sizeof(uint));
				deck->arr[index] = cardValue;
			}
			break;
		}
	}

	deck->count++;
}

void Shuffle(Deck* deck)
{
	if (deck->count > 1)
	{
		size_t i;
		for (i = 0; i < deck->count - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (deck->count - i) + 1); // NOLINT(cert-msc30-c, cert-msc50-cpp)
			uint t = deck->arr[j];
			deck->arr[j] = deck->arr[i];
			deck->arr[i] = t;
		}
	}
}

int GetDeckCount(const Deck* deck)
{
	return deck->count;
}
