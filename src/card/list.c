#include <string.h>
#include "list.h"
#include "constants.h"
#include "utility.h"

static Card cards_internal[MAX_CARDS] = {0};
static Card* cards[MAX_CARDS] = {0};
static int cards_count = 0;
static int next_id = 1;

Card* GetCards(int* outCount)
{
    *outCount = MAX_CARDS;
    return cards_internal;
}

Card** GetCardsOrdered(int* outCount)
{
	*outCount = cards_count;
	return cards;
}

Card* CreateCard(Vector2 position, Suit suit, Rank rank)
{
    if (cards_count == MAX_CARDS - 1)
    {
        TraceLog(LOG_ERROR, "Cannot create card as cards_count is at MAX_CARDS");
        return NULL;
    }

	Card* card = NULL;
	for (int i = 0; i < MAX_CARDS; i++)
	{
		if (cards_internal[i].id) continue;

		card = cards_internal + i;
		break;
	}

	card->id = next_id++;
    card->_position = position;
    card->suit = suit;
    card->rank = rank;
    card->color = ColorFromHSV((float)GetRandomValue(0, 359), 0.2f, 0.8f);

	cards[cards_count] = card;
	cards_count++;

    return card;
}

Card* AddCardValue(unsigned int value)
{
	return CreateCard(V(0,0), value / 13, value % 13);
}

static int GetCardIndex(Card* card)
{
	for (int i = 0; i < cards_count; i++)
	{
		if (cards[i] != card) continue;

		return i;
	}

	TraceLog(LOG_ERROR, "GetCardIndex failed");
	return -1;
}

void DeleteCard(Card *card)
{
	int idx = GetCardIndex(card);
	if (idx < 0 || idx >= cards_count)
	{
		TraceLog(LOG_ERROR, "DeleteCard failed: the card provided is not in cards array");
		return;
	}

	RemoveCardAt(idx);

	*card = (Card){0};
}

Card* RemoveCardAt(int index)
{
	if (index < 0 || index >= cards_count)
	{
		TraceLog(LOG_WARNING, "RemoveCardAt failed: invalid index");
		return NULL;
	}

	Card* card = cards[index];

	memmove(cards + index, cards + index + 1, (cards_count - 1 - index) * sizeof(Card));
	cards_count--;

	return card;
}

void DeleteAllCards(void)
{
	for (int i = 0; i < MAX_CARDS; i++)
	{
		cards_internal[i] = (Card){0};
		cards[i] = NULL;
	}
	cards_count = 0;
}

Card* MoveCardToTop(Card *card)
{
	int idx = GetCardIndex(card);
	if (idx < 0 || idx >= cards_count)
	{
		TraceLog(LOG_ERROR, "MoveCardToTop failed: the card provided is not in cards array");
		return NULL;
	}

	return MoveCardAtToTop(idx);
}

Card* MoveCardAtToTop(int index)
{
	if (index < 0 || index >= cards_count)
	{
		TraceLog(LOG_ERROR, "MoveCardAtToTop failed: invalid index");
		return NULL;
	}

	Card* value = RemoveCardAt(index);

	cards[cards_count] = value;
	cards_count++;

	return cards[cards_count - 1];
}

Card* MousePickCard(Vector2 mpos)
{
	return MousePickCardExcluding(mpos, NULL);
}

Card* MousePickCardExcluding(Vector2 mpos, Card* excluded)
{
	Card* heldCard = NULL;

	for (int i = 0; i < cards_count; ++i)
	{
		Card* card = cards[i];
		if (!card->_locked && CheckCollisionPointRec(mpos, CardGetRect(card)) && card != excluded)
		{
			heldCard = card;
		}
	}

	return heldCard;
}
