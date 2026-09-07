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
#include "cursor.h"

static Card* held_card = NULL;
static Vector2 held_card_offset = {0};

static Card* picked_card = NULL;
static Object* picked_object = NULL;

static Object* held_object = NULL;
static Vector2 held_object_offset = V(0,0);
static Vector2 held_object_last_position = V(0,0);

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
			// TODO: ObjectReservePop can destroy an object, when Cards are Objects this will be a problem!
			// Will need to replace object pointers with IDs before making Cards Objects
			// TODO: picked_object will be invalid after this function is called in cases where the last card is removed!
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
			// TODO: ObjectDiscardPop can destroy an object, when Cards are Objects this will be a problem!
			// Will need to replace object pointers with IDs before making Cards Objects
			// TODO: picked_object will be invalid after this function is called in cases where the last card is removed!
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

	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
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
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		Rectangle objectRec = picked_object ? ObjectRect(picked_object) : R(0,0,0,0);
		if (!held_card && !held_object && picked_object && !picked_object->_locked
			&& mpos.x > (picked_object->_position.x + (objectRec.width * 0.2f))
			)
		{
			Vector2 offset = mpos;
			offset.x = picked_object->_position.x + objectRec.width - OBJECT_HOLD_OFFSET;
			held_object_offset = Vector2Subtract(picked_object->_position, offset);
			held_object_last_position = picked_object->_position;

			held_object = MoveObjectToTop(picked_object);

			ObjectSetHeld(held_object);
		}
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
	{
		if (held_object)
		{
			Rectangle heldObjectRec = RectangleInflate(ObjectRect(held_object), OBJECT_PLACEMENT_INFLATE);
			Object* combineTarget = NULL;
			bool check = true;

			if (picked_object && picked_object->type == held_object->type)
			{
				combineTarget = picked_object;
			}
			else
			{
				// TODO: switch to ordered list if there's any problems
				int objectsCount;
				Object** objects = ObjectsGetOrdered(&objectsCount);
				for (int i = 0; i < objectsCount && check; i++)
				{
					if (!objects[i]->id || objects[i] == held_object) continue;

					if (!CheckCollisionRecs(heldObjectRec, ObjectRect(objects[i]))) continue;

					check = false;
				}
			}

			if (combineTarget)
			{
				if (!ObjectCombine(held_object, combineTarget))
				{
					held_object->_position = held_object_last_position;
				}
				else
				{
					// this object is deleted
					held_object = NULL;
				}
			}
			else if (!check)
			{
				held_object->_position = held_object_last_position;
			}
			else
			{
				// do nothing
			}
			held_object = NULL;
			ObjectSetHeld(NULL);
		}
	}

	if (held_card)
    {
        held_card->_position = Vector2Add(mpos, held_card_offset);
    }
	else if (held_object)
	{
		held_object->_position = Vector2Add(mpos, held_object_offset);
	}

	picked_card = MousePickCardExcluding(mpos, held_card);
	picked_object = MousePickObjectExcluding(mpos, held_object);

	ObjectSetPicked(picked_object);

	if (held_card)
	{
		CursorSetState(CursorHold);
		CursorSetHeight(CursorHeightTop);
	}
	else if (held_object)
	{
		CursorSetState(CursorPalm);
		CursorSetHeight(CursorHeightObject);
	}
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		CursorSetState(CursorPalm);
		CursorSetHeight(CursorHeightTable);
	}
	else if (picked_object || picked_card)
	{
		CursorSetState(CursorPick);
		CursorSetHeight(CursorHeightTop);
	}
	else
	{
		CursorSetState(CursorDefault);
		CursorSetHeight(CursorHeightTop);
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
			Card* card = cards + i;
			DeleteCard(card);
			//ObjectFree(card); // TODO: use this once Cards are objects, as Delete will become Remove and thus will not delete the inner object!
			ReturnCardTo(&main_deck, cards[i], DeckBottom);
		}
	}
	else if (DebugReinitDeck())
	{
		InitDeck(&main_deck);
		DeleteAllCards();
	}

	if (DebugCleanupObjects())
	{
		DeleteAllObjects();
		held_object = NULL;
	}
}

void CardManagerDrawAllCards(void)
{
	float ft = GetFrameTime();
    int cardsCount = 0;
    Card** cards = GetCardsOrdered(&cardsCount);
    for (int i = 0; i < cardsCount; ++i)
    {
        Card* card = cards[i];
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

