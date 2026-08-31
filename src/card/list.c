#include <string.h>
#include "list.h"
#include "constants.h"

static Card cards[MAX_CARDS] = {0};
static int cards_count = 0;

Card* GetCards(int* outCount)
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

    Card* card = cards + cards_count;
    cards_count++;

    card->_position = position;
    card->suit = suit;
    card->rank = rank;
    card->color = ColorFromHSV((float)GetRandomValue(0, 359), 0.2f, 0.8f);

    return card;
}

Card* AddCard(Card card)
{
    if (cards_count == MAX_CARDS - 1)
    {
        TraceLog(LOG_ERROR, "Cannot add card to card list as cards_count is at MAX_CARDS");
        return NULL;
    }

	Card* newCard = cards + cards_count;
    *newCard = card;
    cards_count++;
	return newCard;
}

Card* AddCardValue(unsigned int value)
{
	Card card = {.rank = value % 13, .suit = value / 13};
	return AddCard(card);
}

static int GetCardIndex(Card* card)
{
	size_t idx = ((long)card - (long)cards) / sizeof(Card);
	if (idx >= MAX_CARDS)
	{
		TraceLog(LOG_WARNING, "GetCardIndex failed");
		return -1;
	}

	return (int)idx;
}

void DeleteCard(Card *card)
{
	int idx = GetCardIndex(card);
	if (idx < 0 || idx >= cards_count)
	{
		TraceLog(LOG_ERROR, "DeleteCard failed: the card provided is not in cards array");
		return;
	}

	DeleteCardAt(idx);
}

void DeleteCardAt(int index)
{
	if (index < 0 || index >= cards_count)
	{
		TraceLog(LOG_WARNING, "DeleteCardAt failed: invalid index");
		return;
	}

	memmove(cards + index, cards + index + 1, (cards_count - 1 - index) * sizeof(Card));
	cards_count--;
}

void DeleteAllCards(void)
{
	for (int i = 0; i < MAX_CARDS; i++)
	{
		cards[i] = (Card){0};
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

	Card copy = cards[index];
	DeleteCardAt(index);

	cards[cards_count] = copy;
	cards_count++;

	return cards + cards_count - 1;
}

Card* MousePickCard(Vector2 mpos)
{
    Card* heldCard = NULL;

    for (int i = 0; i < cards_count; ++i)
    {
        Card* card = cards + i;
        if (!card->_locked && CheckCollisionPointRec(mpos, CardGetRect(card)))
        {
            heldCard = card;
        }
    }

    return heldCard;
}
