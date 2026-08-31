#include "manager.h"
#include "utility.h"
#include "raymath.h"
#include "constants.h"
#include "debug.h"
#include "list.h"
#include "utility/tween.h"
#include "lock_timers.h"
#include "card/deck.h"

static Card* held_card = NULL;
static Vector2 held_card_offset = {0};

const Card* GetHeldCard(void)
{
    return held_card;
}

void CardManagerInit(void)
{
    held_card = NULL;
	InitDeck();
}

void CardManagerUpdate(float ft)
{
    Vector2 mpos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        held_card = MousePickCard(mpos);

        if (held_card)
        {
            held_card_offset = Vector2Subtract(held_card->_position, mpos);

			Card* temp = MoveCardToTop(held_card);
			if (temp)
			{
				held_card = temp;
			}
        }
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        held_card = NULL;
    }

    if (held_card)
    {
        held_card->_position = Vector2Add(mpos, held_card_offset);
    }

    // DEBUG
	if (DebugSpawnCard())
    {
//        Card* newCard = CreateCard(
//            V(GetScreenWidth() * 0.5f, -200),
//            GetRandomValue(0, 3),
//            GetRandomValue(0, 12)
//        );

		if (GetDeckCount() > 0)
		{
			Card* newCard = AddCardValue(DrawNewCardValue());

			Vector2 cardSize = CardGetSize(newCard);
			Vector2 startPosition = V((GetScreenWidth() * 0.5f) - (cardSize.x * 0.5f), 32 - cardSize.y);
			Vector2 cardDest = startPosition;
			cardDest.y = 64;
			newCard->_position = startPosition;

			/* TODO: Tweening card values directly will not work, as cards get moved around and thus the pointer to their
			 * values will change. Better to use a dedicated "Card Tweener" and capture a Card "ID", which could not change. */
			SetTweenStyle(TweenStyleEaseOut);
			SetTweenV2(&newCard->_position, cardDest, 0.5f);
			LockCardFor(newCard, 0.5f);
		}
		else
		{
			TraceLog(LOG_WARNING, "No cards left in deck!");
		}
    }

	if (DebugDeckTest())
	{
		TraceLog(LOG_INFO, "");
		TraceLog(LOG_INFO, "");
		TraceLog(LOG_INFO, "");

		int deckCount = GetDeckCount();
		for (int i = 0; i < deckCount; i++)
		{
			Card card = DrawNewCard();
			TraceLog(LOG_INFO, "----======----");
			TraceLog(LOG_INFO, "%s of %s", RankName(card.rank), SuitName(card.suit));
		}
	}
	else if (DebugShuffleDeck())
	{
		Shuffle();
	}
	else if (DebugClearCards())
	{
		int count;
		Card* cards = GetCards(&count);
		for (int i = count - 1; i >= 0; i--)
		{
			DeleteCard(cards + i);
			ReturnCardTo(cards[i], DeckBottom);
		}
	}
	else if (DebugReinitDeck())
	{
		InitDeck();
		DeleteAllCards();
	}
}

void CardManagerDrawAllCards(void)
{
    int cardsCount = 0;
    Card* cards = GetCards(&cardsCount);
    for (int i = 0; i < cardsCount; ++i)
    {
        Card* card = cards + i;
		float alpha = card->_locked ? 0.6f : 1.0f;
        CardDraw(card, alpha);
    }
}

