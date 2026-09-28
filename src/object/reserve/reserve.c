#include <string.h>
#include "object/reserve.h"
#include "object.h"
#include "object/management.h"
#include "object/card/resources.h"
#include "utility.h"
#include "debug.h"

Object* ObjectCreateReserve(Vector2 position)
{
	Object* object = ObjectConstruct();
	object->type = ObjectReserve;
	object->_position = position;

	object->data.reserve.deck = (Deck){0};
	object->data.reserve._faceUp = false;

	return object;
}

static float ReserveStackOffset(void)
{
	return DebugShowOneToOneStacks() ? 1.0f : RESERVE_STACK_OFFSET;
}

static void ObjectReserveDrawInternal(const Object* object, bool shadowed, Color highlight)
{
	if (object->type != ObjectReserve)
	{
		TraceLog(LOG_ERROR, "ObjectReserveDrawHighlight called on Object of non-Reserve type!");
		return;
	}

	// TODO: implement highlight
	int stackHeight = 1;
	float offset = ReserveStackOffset();

	bool small = DebugDrawCardsSmall();
	Vector2 cardSize = small ? CARD_SIZE_SMALL : CARD_SIZE;
	Rectangle dest = R(object->_position.x, object->_position.y, cardSize.x, cardSize.y);
	Texture2D tex = GetCardBack(small);
	Rectangle src = GetCardSource(small);

	if (shadowed)
	{
		Rectangle shadowDest = dest;

		dest.x -= OBJECT_SHADOW_OFFSET;
		dest.y -= OBJECT_SHADOW_OFFSET;

		DrawRectangleRounded(shadowDest, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, Fade(BLACK, OBJECT_SHADOW_DARKNESS));
	}
	else if (!ColorIsEqual(highlight, BLANK))
	{
		Rectangle highlightRec = RectangleInflate(dest, CARD_HIGHLIGHT_EXTENT);
		DrawRectangleRounded(highlightRec, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, highlight);
	}

	stackHeight = ObjectReserveStackHeight(object);

	while (stackHeight > 0)
	{
		int deckIndex = stackHeight - 1;
		if (DebugShowOneToOneStacks() && deckIndex == object->data.reserve.scrollIndex
			&& object->data.reserve.scrollIndex != 0)
		{
			Rectangle altDest = dest;
			altDest.y -= RESERVE_SCROLL_OFFSET;
			const Deck* deck = &object->data.reserve.deck;
			if (object->data.reserve._faceUp)
			{
				int value = (int)deck->arr[deck->count - 1 - object->data.reserve.scrollIndex];
				Texture2D texAlt = GetCardValue(value, small);
				DrawTexturePro(texAlt, src, altDest, V(0,0), 0.0f, WHITE);
			}
			else
			{
				DrawTexturePro(tex, src, altDest, V(0,0), 0.0f, WHITE);
			}
			DrawText(TextFormat("%d/%d", object->data.reserve.scrollIndex + 1, object->data.reserve.deck.count), (int)altDest.x, (int)altDest.y - 16, 16, BLACK);
		}
		else if (stackHeight == 1 && object->data.reserve._faceUp)
		{
			int value = (int)object->data.reserve.deck.arr[object->data.reserve.deck.count - 1];
			Texture2D texAlt = GetCardValue(value, small);
			DrawTexturePro(texAlt, src, dest, V(0,0), 0.0f, WHITE);
		}
		else
		{
			DrawTexturePro(tex, src, dest, V(0,0), 0.0f, WHITE);
		}

		dest.y -= offset;
		stackHeight--;
	}
}

void ReserveTick(Object* object, float ft)
{

}

void ReserveDraw(const Object* object)
{
	ObjectReserveDrawInternal(object, ObjectGetDrawShadowed(), ObjectGetDrawHighlight());
}

Rectangle ReserveRect(Object* object)
{
	Vector2 sz = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
	float yOff = (float)(ObjectReserveStackHeight(object) - 1) * ReserveStackOffset();
	return R(object->_position.x, object->_position.y - yOff, sz.x, sz.y + yOff);
}

ObjectCombinationResult ReserveCombine(Object* source, Object* destination)
{
	ObjectCombinationResult result = {0};

	Deck* destDeck = NULL;
	Deck* sourceDeck = &source->data.reserve.deck;
	// TODO: do we sometimes want to reverse the source deck depending on what the destination type is?

	switch (destination->type)
	{
		case ObjectCard:
			if (!destination->data.card._faceUp)
			{
				Object* reserve = ObjectCreateReserve(destination->_position);
				destDeck = &(reserve->data.reserve.deck);

				ReturnCardTo(destDeck, destination->data.card.value, DeckTop);
				ObjectFree(destination);

				result.type |= OCR_NewObject;
			}
			else
			{
				Object* discard = ObjectCreateDiscard(destination->_position);
				destDeck = &(discard->data.discard.deck);

				ReturnCardTo(destDeck, destination->data.card.value, DeckTop);
				ObjectFree(destination);

				result.type |= OCR_NewObject;
			}
			break;
		case ObjectReserve:
			destDeck = &destination->data.reserve.deck;
			break;
		case ObjectDiscard:
			destDeck = &destination->data.discard.deck;
			break;
		default:
			TraceLog(LOG_ERROR, "ReserveCombine unhandled object type, no action was taken.");
			return result;
	}

	// TODO: does this logic need to be moved to a utility function? see discard.c
	int sourceDeckCount = GetDeckCount(sourceDeck);
	int destDeckRemainingCap = GetDeckMax(destDeck) - GetDeckCount(destDeck);
	if (destDeckRemainingCap >= sourceDeckCount)
	{
		memmove(destDeck->arr + destDeck->count, sourceDeck->arr, sourceDeckCount * sizeof(uint));
		destDeck->count += sourceDeckCount;
		RemoveObject(source);

		result.type |= OCR_SourceDestroyed;
		return result;
	}
	else if (DeckIsFull(destDeck))
	{
		return result;
	}
	else
	{
		memmove(destDeck->arr + destDeck->count, sourceDeck->arr, destDeckRemainingCap * sizeof(uint));
		destDeck->count += destDeckRemainingCap;
		memmove(sourceDeck->arr, sourceDeck->arr + destDeckRemainingCap, (sourceDeckCount - destDeckRemainingCap) * sizeof(uint));
		sourceDeck->count -= destDeckRemainingCap;

		result.type |= OCR_Success;
		return result;
	}
}

bool ReserveFlippable(const Object* object)
{
	// TODO: objects should hold "allowed actions" flags. locked is not detailed enough: perhaps an object
	// can be drawn from but not moved or flipped, as example.
	return object->_locked;
}

void ReserveFlip(Object* object)
{
	// TODO: animation?
	DeckReverse(&object->data.reserve.deck);
	object->data.reserve._faceUp = !object->data.reserve._faceUp;
}

void ReserveHandlePickup(Object* object)
{
	// stub
	// TODO: likely to be used for animation
}

void ReserveHandleDrop(Object* object)
{
	// stub
	// TODO: likely to be used for animation
	object->data.reserve.scrollIndex = 0;
}

ObjectInteractionResult ReserveHandleInput(Object* object, Input input)
{
	ObjectInteractionResult result = {0};
	switch (input)
	{
		case InputPrimary:
			if (object->_held)
			{
				result.type = OIR_ObjectCreated;
				result.object = ObjectReservePop(object);

				if (object->id)
				{
					// ReservePop pushes new object to top, so call this again to keep held object on top.
					MoveObjectToTop(object);
				}
				else
				{
					result.type = OIR_HeldObjectDestroyed;
				}
			}
			else
			{
				result.type = OIR_ObjectCreatedToHold;
				result.object = ObjectReservePop(object);
			}
			break;
		case InputSecondary:
			// Should I be called ObjectFlip here? or is that unneeded indirection?
			ReserveFlip(object);
			break;
		case InputScrollDown:
			if (object->data.reserve.scrollIndex < object->data.reserve.deck.count - 1)
			{
				object->data.reserve.scrollIndex++;
			}
			break;
		case InputScrollUp:
			if (object->data.reserve.scrollIndex > 0)
			{
				object->data.reserve.scrollIndex--;
			}
			break;
		default:
			break;
	}
	return result;
}

void ReserveHandlePicked(Object* object, bool picked)
{
	if (picked) return;

    if (!object->_held)
    {
	    object->data.reserve.scrollIndex = 0;
    }
}

void ObjectReserveDrawShadowed(const Object* object)
{
	ObjectReserveDrawInternal(object, true, BLANK);
}

void ObjectReserveDrawHighlight(const Object* object, Color highlight)
{
	ObjectReserveDrawInternal(object, false, highlight);
}

bool ObjectReserveFull(const Object* object)
{
	if (object->type != ObjectReserve)
	{
		TraceLog(LOG_TRACE, "ObjectReserveFull called on Object of non-Reserve type!");
		return true;
	}

	return DeckIsFull(&(object->data.reserve.deck));
}

int ObjectReserveStackHeight(const Object* object)
{
	if (object->type != ObjectReserve)
	{
		TraceLog(LOG_ERROR, "ObjectReserveStackHeight called on Object of non-Reserve type!");
		return -1;
	}

	return DebugShowOneToOneStacks() ? object->data.reserve.deck.count : CLAMPf(object->data.reserve.deck.count, 1, 4);
}

Object* ObjectReservePop(Object* object)
{
	if (object->type != ObjectReserve)
	{
		TraceLog(LOG_ERROR, "ObjectReservePop called on Object of non-Reserve type!");
		return NULL;
	}

	Deck* deck = &(object->data.reserve.deck);

	// TODO redefine card.value type as CardValue defined in utility/types to avoid uint / int narrowing?
	uint value = DebugShowOneToOneStacks() ? DrawFrom(deck, deck->count - 1 - object->data.reserve.scrollIndex)
			: DrawNewCardValue(deck);
	Object* card = ObjectCreateCard(object->_position, (int)value);

	card->_position = object->_position;
	card->data.card._animationState = CardStateDefault;
	card->data.card._faceUp = object->data.reserve._faceUp;

	if (DebugShowOneToOneStacks())
	{
		if (object->_held && object->data.reserve.scrollIndex == object->data.reserve.deck.count)
		{
			object->data.reserve.scrollIndex--;
		}
//		else
//		{
//			object->data.reserve.scrollIndex = 0;
//		}
	}

	// Destroy reserve if the last card is popped
	if (GetDeckCount(deck) == 0)
	{
		RemoveObject(object);
		ObjectFree(object);
	}

	return card;
}
