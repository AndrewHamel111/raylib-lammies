#include "management.h"
#include "raymath.h"
#include "debug.h"
#include "utility.h"
#include "reserve.h"
#include "discard.h"
#include "object/card/animation.h"
#include "cursor.h"

static Object* picked_object = NULL;

static Object* held_object = NULL;
static Vector2 held_object_offset = {0};
static Vector2 held_object_last_position = {0};

typedef enum HoldMode
{
	HoldPick = 0,
	HoldPalm,
} HoldMode;

static HoldMode hold_mode = HoldPick;

static bool trying_pickup = false;

void PlayerInteractionUpdate(float ft)
{
	Vector2 mpos = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (!held_object && picked_object && picked_object->type == ObjectCard)
		{
			Object* temp = MoveObjectToTop(picked_object);
			if (temp)
			{
				held_object = temp;
				hold_mode = HoldPick;
				held_object_offset = Vector2Subtract(held_object->_position, mpos);
			}
		}
		else if (!held_object && picked_object && picked_object->type == ObjectReserve)
		{
			held_object = ObjectReservePop(picked_object);
			hold_mode = HoldPick;
			if (!held_object)
			{
				TraceLog(LOG_WARNING, "ObjectTypeReserve is picked on MouseLeft, but ObjectReservePop failed!");
			}
			else
			{
				Object* temp = MoveObjectToTop(held_object);
				if (temp)
				{
					held_object = temp;
					held_object_offset = Vector2Subtract(held_object->_position, mpos);
				}
				else
				{
					TraceLog(LOG_ERROR, "Failed to move held object to top! held_object may be invalid");
				}
			}
		}
		else if (!held_object && picked_object && picked_object->type == ObjectDiscard)
		{
			held_object = ObjectDiscardPop(picked_object);
			if (!held_object)
			{
				TraceLog(LOG_WARNING, "ObjectTypeDiscard is picked on MouseLeft, but ObjectDiscardPop failed!");
			}
			else
			{
				Object* temp = MoveObjectToTop(held_object);
				if (temp)
				{
					held_object = temp;
					held_object_offset = Vector2Subtract(held_object->_position, mpos);
				}
				else
				{
					TraceLog(LOG_ERROR, "Failed to move held object to top! held_object may be invalid");
				}
			}
		}
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		bool cardHeld = held_object && held_object->type == ObjectCard;
		bool cardPicked = picked_object && picked_object->type == ObjectCard;
		bool faceDownCardPicked = cardPicked && !picked_object->data.card._faceUp;
		bool faceUpCardPicked = cardPicked && picked_object->data.card._faceUp;
		if (cardHeld && faceDownCardPicked)
		{
			Object* reserve = ObjectCreateReserve(picked_object->_position);
			Deck* deck = &(reserve->data.reserve.deck);

			ReturnCardTo(deck, picked_object->data.card.value, DeckTop);
			ReturnCardTo(deck, held_object->data.card.value, DeckTop);

			ObjectFree(held_object);
			ObjectFree(picked_object);
			picked_object = NULL;
		}

			// create discard
		else if (cardHeld && faceUpCardPicked)
		{
			Object* reserve = ObjectCreateDiscard(picked_object->_position);
			Deck* deck = &(reserve->data.discard.deck);

			ReturnCardTo(deck, picked_object->data.card.value, DeckTop);
			ReturnCardTo(deck, held_object->data.card.value, DeckTop);

			ObjectFree(held_object);
			ObjectFree(picked_object);
			picked_object = NULL;
		}

			// add card to reserve
		else if (cardHeld && picked_object && !ObjectReserveFull(picked_object))
		{
			Deck* reserveDeck = &(picked_object->data.reserve.deck);

			ReturnCardTo(reserveDeck, held_object->data.card.value, DeckTop);
			ObjectFree(held_object);
		}

			// add card to discard
		else if (cardHeld && picked_object && !ObjectDiscardFull(picked_object))
		{
			Deck* discardDeck = &(picked_object->data.discard.deck);

			ReturnCardTo(discardDeck, held_object->data.card.value, DeckTop);
			ObjectFree(held_object);
		}

		held_object = NULL;
	}

	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
//		if (held_object && held_object->type == ObjectCard)
//		{
//			CardFlip(held_object);
//		} else
		if (!held_object && picked_object && picked_object->type == ObjectCard)
		{
			CardFlip(picked_object);
		}
		else if (!held_object && !picked_object)
		{
			trying_pickup = true;
		}
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
	{
		trying_pickup = false;

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
					// this object is deleted by ObjectCombine
					held_object = NULL;
				}
			}
			else if (!check && held_object->type != ObjectCard)
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
	else if (trying_pickup)
	{
		Rectangle objectRec = picked_object ? ObjectRect(picked_object) : R(0,0,0,0);
		if (!held_object && picked_object && !picked_object->_locked
			&& mpos.x > (picked_object->_position.x + (objectRec.width * 0.2f))
				)
		{
			Vector2 offset = mpos;
			offset.x = picked_object->_position.x + objectRec.width - OBJECT_HOLD_OFFSET;
			held_object_offset = Vector2Subtract(picked_object->_position, offset);
			held_object_last_position = picked_object->_position;

			held_object = MoveObjectToTop(picked_object);
			hold_mode = HoldPalm;

			ObjectSetHeld(held_object);
		}
	}

	if (held_object)
	{
		held_object->_position = Vector2Add(mpos, held_object_offset);
	}

	picked_object = MousePickObjectExcluding(mpos, held_object);

	ObjectSetPicked(picked_object);

	if (held_object && hold_mode == HoldPick)
	{
		CursorSetState(CursorHold);
		CursorSetHeight(CursorHeightTop);
	}
	else if (held_object)
	{
		CursorSetState(CursorPalm);
		CursorSetHeight(CursorHeightObject);
	}
	else if (trying_pickup)
	{
		CursorSetState(CursorPalm);
		CursorSetHeight(CursorHeightTable);
	}
	else if (picked_object)
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
	if (DebugSpawnFactoryDeck())
	{
		Object* reserve = ObjectCreateReserve(V(128, 128));
		Deck* deck = &reserve->data.reserve.deck;
		InitDeck(deck);
	}
	else if (DebugSpawnShuffledDeck())
	{
		Object* reserve = ObjectCreateReserve(V(128, 128));
		Deck* deck = &reserve->data.reserve.deck;
		InitDeck(deck);
		Shuffle(deck);
	}
//	else if (DebugClearCards())
//	{
		// TODO: if we want to keep this, will need to write new logic
//	}
	else if (DebugCleanupObjects())
	{
		DeleteAllObjects();
		held_object = NULL;
	}
}
