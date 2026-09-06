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
#include "object.h"
#include "object/management.h"
#include "object/reserve.h"
#include "object/discard.h"

static Card* held_card = NULL;
static Vector2 held_card_offset = {0};

static Card* picked_card = NULL;
static Object* picked_object = NULL;

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
			Card* temp = MoveCardToTop(held_card);
			if (temp)
			{
				held_card = temp;
				held_card_offset = Vector2Subtract(held_card->_position, mpos);
			}
        }
		else if (picked_object && picked_object->type == ObjectTypeReserve)
		{
			held_card = ObjectReservePop(picked_object);
			if (!held_card)
			{
				TraceLog(LOG_WARNING, "ObjectTypeReserve is picked on MouseLeft, but ObjectReservePop failed!");
			}
			else
			{
				Card* temp = MoveCardToTop(held_card);
				if (temp)
				{
					held_card = temp;
					held_card_offset = Vector2Subtract(held_card->_position, mpos);
				}
			}
		}
		else if (picked_object && picked_object->type == ObjectTypeDiscard)
		{
			held_card = ObjectDiscardPop(picked_object);
			if (!held_card)
			{
				TraceLog(LOG_WARNING, "ObjectTypeDiscard is picked on MouseLeft, but ObjectDiscardPop failed!");
			}
			else
			{
				Card* temp = MoveCardToTop(held_card);
				if (temp)
				{
					held_card = temp;
					held_card_offset = Vector2Subtract(held_card->_position, mpos);
				}
			}
		}
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
		// TODO: doesn't work since cards move when the cards list is being updated, so pointers get invalidated.
		// Need to move away from using pointers to an array that changes. Maybe use a "card handle" type which
		// is just the ID, and then lookup the ID before each operation?
		if (held_card && picked_card && !picked_card->_faceUp)
		{
			Object* reserve = ObjectReserveCreate(picked_card->_position);
			Deck* deck = &(reserve->data.reserve.deck);

			ReturnCardTo(deck, *picked_card, DeckTop);
			ReturnCardTo(deck, *held_card, DeckTop);

			DeleteCard(held_card);
			DeleteCard(picked_card);
		}

		// create discard
		else if (held_card && picked_card && picked_card->_faceUp)
		{
			Object* reserve = ObjectDiscardCreate(picked_card->_position);
			Deck* deck = &(reserve->data.discard.deck);

			ReturnCardTo(deck, *picked_card, DeckTop);
			ReturnCardTo(deck, *held_card, DeckTop);

			DeleteCard(held_card);
			DeleteCard(picked_card);
		}

		// add card to reserve
		else if (held_card && picked_object && !ObjectReserveFull(picked_object))
		{
			Object* reserve = picked_object;
			Deck* deck = &(reserve->data.reserve.deck);

			ReturnCardTo(deck, *held_card, DeckTop);
			DeleteCard(held_card);
		}

		// add card to discard
		else if (held_card && picked_object && !ObjectDiscardFull(picked_object))
		{
			Object* discard = picked_object;
			Deck* deck = &(discard->data.discard.deck);

			ReturnCardTo(deck, *held_card, DeckTop);
			DeleteCard(held_card);
		}

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

	picked_card = MousePickCardExcluding(mpos, held_card);
	picked_object = MousePickObject(mpos);

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
		if (held_card == card)
		{
			CardDrawShadowed(card, alpha);
		}
		else if (picked_card == card)
		{
			CardDrawHighlight(card, alpha, SKYBLUE);
		}
		else
		{
        	CardDraw(card, alpha);
		}

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

