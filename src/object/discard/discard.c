#include <string.h>
#include "object.h"
#include "object/discard.h"
#include "object/management.h"
#include "debug.h"
#include "object/card/resources.h"
#include "utility.h"

#include "rlgl.h"
#include "object/card.h"

static float card_rotations[] = { -5.0f, 8.0f, -3.0f, 11.0f, -8.0f, 4.0f, -7.0f, 6.0f, -9.0f, 10.0f, -13.0f, 2.0f };

Object* ObjectCreateDiscard(Vector2 position)
{
	Object* object = ObjectConstruct();
	object->type = ObjectDiscard;
	object->_position = position;

	object->data.discard.deck = (Deck){0};
	object->data.discard._faceUp = true;

	return object;
}

static float DiscardStackOffset(void)
{
	return DebugShowOneToOneStacks() ? 1.0f : DISCARD_STACK_OFFSET;
}

static void ObjectDiscardDrawInternal(const Object* object, bool shadowed, Color highlight)
{
	if (object->type != ObjectDiscard)
	{
		TraceLog(LOG_ERROR, "ObjectDiscardDrawHighlight called on Object of non-Discard type!");
		return;
	}

	// TODO: implement highlight

	int stackHeight = ObjectDiscardStackHeight(object);
	float offset = DiscardStackOffset();
	const Deck* deck = &object->data.discard.deck;

	Vector2 position = object->_position;

	if (shadowed)
	{
		Rectangle objectRec = ObjectRect(object);
		position.x -= OBJECT_SHADOW_OFFSET;
		position.y -= OBJECT_SHADOW_OFFSET;
		DrawRectangleRounded(objectRec, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, Fade(BLACK, OBJECT_SHADOW_DARKNESS));
	}

//	Rectangle highlightRec = RectangleInflate(dest, CARD_HIGHLIGHT_EXTENT);
//	DrawRectangleRounded(highlightRec, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, highlight);

	bool drawHighlight = !ColorIsEqual(highlight, BLANK);

	int i = 0;
	while (stackHeight > 0)
	{
		Vector2 cardPosition = position;
		cardPosition.y -= offset * (float)i;

		int deckIndex = stackHeight - 1;
		if (DebugShowOneToOneStacks() && deckIndex == object->data.discard.scrollIndex
			&& object->data.discard.scrollIndex != 0)
		{
			cardPosition.y -= DISCARD_SCROLL_OFFSET;

			if (drawHighlight)
			{
				rlPushMatrix();
				{
					Vector2 cardSize = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
					rlTranslatef(cardPosition.x + cardSize.x / 2, cardPosition.y + cardSize.y / 2, 0);
					rlRotatef(card_rotations[i % 12], 0, 0, 1);
					rlTranslatef(-cardSize.x / 2, -cardSize.y / 2, 0);

					Rectangle dest = RectangleInflate(R(0, 0, cardSize.x, cardSize.y), CARD_HIGHLIGHT_EXTENT);
					DrawRectangleRounded(dest, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, highlight);
				}
				rlPopMatrix();
			}

			DrawText(TextFormat("%d/%d", object->data.discard.scrollIndex + 1, object->data.discard.deck.count), (int)cardPosition.x, (int)cardPosition.y - 24, 16, BLACK);
		}
		else if (stackHeight == 1 && drawHighlight && (!DebugShowOneToOneStacks() || object->data.discard.scrollIndex == 0))
		{
			rlPushMatrix();
			{
				Vector2 cardSize = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
				rlTranslatef(cardPosition.x + cardSize.x / 2, cardPosition.y + cardSize.y / 2, 0);
				rlRotatef(card_rotations[i % 12], 0, 0, 1);
				rlTranslatef(-cardSize.x / 2, -cardSize.y / 2, 0);

				Rectangle dest = RectangleInflate(R(0, 0, cardSize.x, cardSize.y), CARD_HIGHLIGHT_EXTENT);
				DrawRectangleRounded(dest, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, highlight);
			}
			rlPopMatrix();
		}

		if (object->data.discard._faceUp)
		{
			int value = (int)deck->arr[deck->count - stackHeight];
			CardDrawCustom(cardPosition, value, card_rotations[i % 12], BLANK);
		}
		else
		{
			CardDrawBack(cardPosition, card_rotations[i % 12], BLANK);
		}
		i++;
		stackHeight--;
	}
}

void DiscardTick(Object* object, float ft)
{
	// do nothing
}

void DiscardDraw(const Object* object)
{
	ObjectDiscardDrawInternal(object, ObjectGetDrawShadowed(), ObjectGetDrawHighlight());
}

Rectangle DiscardRect(Object* object)
{
	Vector2 sz = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
	float yOff = (float)(ObjectDiscardStackHeight(object) - 1) * DiscardStackOffset();
	return R(object->_position.x, object->_position.y - yOff, sz.x, sz.y + yOff);
}

ObjectCombinationResult DiscardCombine(Object* source, Object* destination)
{
	ObjectCombinationResult result = {0};

	Deck* destDeck = NULL;
	Deck* sourceDeck = &source->data.discard.deck;
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
			TraceLog(LOG_ERROR, "DiscardCombine unhandled object type, no action was taken.");

			return result;
	}

	// TODO: does this logic need to be moved to a utility function? see reserve.c
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

bool DiscardFlippable(const Object* object)
{
	// TODO: objects should hold "allowed actions" flags. locked is not detailed enough: perhaps an object
	// can be drawn from but not moved or flipped, as example.
	return !object->_locked;
}

void DiscardFlip(Object* object)
{
	// TODO: animation?
	DeckReverse(&object->data.discard.deck);
	object->data.discard._faceUp = !object->data.discard._faceUp;
}

void DiscardHandlePickup(Object* object)
{
	// stub
	// TODO: likely to be used for animation
}

void DiscardHandleDrop(Object* object)
{
	// stub
	// TODO: likely to be used for animation
	object->data.discard.scrollIndex = 0;
}

ObjectInteractionResult DiscardHandleInput(Object* object, Input input)
{
	ObjectInteractionResult result = {0};

	switch (input)
	{
		case InputPrimary:
			if (object->_held)
			{
				result.type = OIR_ObjectCreated;
				result.object = ObjectDiscardPop(object);

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
				result.object = ObjectDiscardPop(object);
			}
			break;
		case InputSecondary:
			// Should I be called ObjectFlip here? or is that unneeded indirection?
			DiscardFlip(object);
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

void DiscardHandlePicked(Object* object, bool picked)
{
	if (picked) return;

	object->data.discard.scrollIndex = 0;
}

bool ObjectDiscardFull(const Object* object)
{
	if (object->type != ObjectDiscard)
	{
		TraceLog(LOG_TRACE, "ObjectDiscardFull called on Object of non-Discard type!");
		return true;
	}

	const Deck* deck = &object->data.discard.deck;
	return GetDeckCount(deck) == GetDeckMax(deck);
}

int ObjectDiscardStackHeight(const Object* object)
{
	if (object->type != ObjectDiscard)
	{
		TraceLog(LOG_ERROR, "ObjectDiscardStackHeight called on Object of non-Discard type!");
		return true;
	}

	return DebugShowOneToOneStacks() ? object->data.discard.deck.count : CLAMPf(object->data.discard.deck.count, 1, 12);
}

Object* ObjectDiscardPop(Object* object)
{
	if (object->type != ObjectDiscard)
	{
		TraceLog(LOG_ERROR, "ObjectDiscardPop called on Object of non-Discard type!");
		return NULL;
	}

	Deck* deck = &(object->data.discard.deck);

	// TODO redefine card.value type as CardValue defined in utility/types to avoid uint / int narrowing?
//	Object* card = ObjectCreateCard(object->_position, DrawNewCardValue(deck));
	uint value = DebugShowOneToOneStacks() ? DrawFrom(deck, deck->count - 1 - object->data.discard.scrollIndex)
										   : DrawNewCardValue(deck);
	Object* card = ObjectCreateCard(object->_position, (int)value);

	card->_position = object->_position;
	card->data.card._animationState = CardStateDefault;
	card->data.card._faceUp = object->data.discard._faceUp;

	if (DebugShowOneToOneStacks())
	{
		if (object->_held)
		{
			object->data.discard.scrollIndex--;
		}
		else
		{
			object->data.discard.scrollIndex = 0;
		}
	}

	// Destroy discard if the last card is popped
	if (GetDeckCount(deck) == 0)
	{
		RemoveObject(object);
		ObjectFree(object);
	}

	return card;
}
