#include "manager.h"
#include "utility.h"
#include "raymath.h"
#include "constants.h"
#include "debug.h"
#include "list.h"
#include "utility/tween.h"
#include "lock_timers.h"
#include "card/deck.h"
#include "card/animation.h"

static Card* held_card = NULL;
static Vector2 held_card_offset = {0};

static Deck main_deck;

const Card* GetHeldCard(void)
{
    return held_card;
}

void CardManagerInit(void)
{
    held_card = NULL;
	InitDeck(&main_deck);
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

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		if (held_card)
		{
			CardFlip(held_card);
		}
		else
		{
			Card* cardToFlip = MousePickCard(mpos);
			if (cardToFlip)
			{
				CardFlip(cardToFlip);
			}
		}
	}

    if (held_card)
    {
        held_card->_position = Vector2Add(mpos, held_card_offset);
    }

    // DEBUG
	if (DebugSpawnCard())
    {
		if (GetDeckCount(&main_deck) > 0)
		{
			Card* newCard = AddCardValue(DrawNewCardValue(&main_deck));

			Vector2 cardSize = CardGetSize(newCard);
			Vector2 startPosition = V((GetScreenWidth() * 0.5f), 32 - cardSize.y);
			Vector2 cardDest = startPosition;
			cardDest.y = 64 + (cardSize.y / 2);
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

		int deckCount = GetDeckCount(&main_deck);
		for (int i = 0; i < deckCount; i++)
		{
			Card card = DrawNewCard(&main_deck);
			TraceLog(LOG_INFO, "----======----");
			TraceLog(LOG_INFO, "%s of %s", RankName(card.rank), SuitName(card.suit));
		}
	}
	else if (DebugShuffleDeck())
	{
		Shuffle(&main_deck);
	}
	else if (DebugClearCards())
	{
		int count;
		Card* cards = GetCards(&count);
		for (int i = count - 1; i >= 0; i--)
		{
			DeleteCard(cards + i);
			ReturnCardTo(&main_deck, cards[i], DeckBottom);
		}
	}
	else if (DebugReinitDeck())
	{
		InitDeck(&main_deck);
		DeleteAllCards();
	}
}

void CardManagerDrawAllCards(void)
{
	float ft = GetFrameTime();
    int cardsCount = 0;
    Card* cards = GetCards(&cardsCount);
    for (int i = 0; i < cardsCount; ++i)
    {
        Card* card = cards + i;
		float alpha = card->_locked ? 0.6f : 1.0f;
        CardDraw(card, alpha);

		if (card->_animationState != CardStateDefault)
		{
			card->_flipTime -= ft;
			if (card->_flipTime < 0)
			{
				if (card->_animationState == CardStateFlippingDownIn || card->_animationState == CardStateFlippingUpIn)
				{
					card->_faceUp = ! card->_faceUp;
					card->_flipTime = CARD_FLIP_TIME;
					card->_animationState++;
				}
				else
				{
					card->_animationState = CardStateDefault;
				}
			}
		}
    }
}

