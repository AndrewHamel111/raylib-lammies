#include <string.h>
#include "object.h"
#include "reserve.h"
#include "utility.h"
#include "debug.h"
#include "discard.h"
#include "management.h"

void ObjectDraw(const Object* object)
{
	switch (object->type)
	{
		case ObjectTypeReserve:
			ObjectReserveDraw(object);
			return;
		case ObjectTypeDiscard:
			ObjectDiscardDraw(object);
			return;
	}

	TraceLog(LOG_WARNING, "ObjectDraw unhandled case for object->type %d", object->type);
}

void ObjectDrawHighlight(const Object* object, Color highlight)
{
	switch (object->type)
	{
		case ObjectTypeReserve:
			ObjectReserveDrawHighlight(object, highlight);
			break;
		case ObjectTypeDiscard:
			ObjectDiscardDrawHighlight(object, highlight);
			break;
	}
}

void ObjectDrawShadowed(const Object* object)
{
	switch (object->type)
	{
		case ObjectTypeReserve:
			ObjectReserveDrawShadowed(object);
			break;
		case ObjectTypeDiscard:
			ObjectDiscardDrawShadowed(object);
			break;
	}
}

Rectangle ObjectRect(const Object* object)
{
	switch (object->type)
	{
		// TODO: make these constants and update card.c accordingly
		case ObjectTypeReserve:
		{
			Vector2 sz = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
			float yOff = (float)(ObjectReserveStackHeight(object) - 1) * RESERVE_STACK_OFFSET;
			return R(object->_position.x, object->_position.y - yOff, sz.x, sz.y + yOff);
		}
		case ObjectTypeDiscard:
		{
			Vector2 sz = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
			float yOff = (float)(ObjectDiscardStackHeight(object) - 1) * DISCARD_STACK_OFFSET;
			return R(object->_position.x, object->_position.y - yOff, sz.x, sz.y + yOff);
		}
	}

	TraceLog(LOG_WARNING, "ObjectRect unhandled case for object->type %d", object->type);
	return R(0,0,0,0);
}

bool ObjectCombine(Object* source, Object* destination)
{
	if (source->type != destination->type)
	{
		TraceLog(LOG_ERROR, "ObjectCombine called with mismatched object types!");
		return false;
	}

	Deck* destDeck;
	Deck* sourceDeck;

	switch (source->type)
	{
		case ObjectTypeReserve:
			destDeck = &destination->data.reserve.deck;
			sourceDeck = &source->data.reserve.deck;
			break;
		case ObjectTypeDiscard:
			destDeck = &destination->data.discard.deck;
			sourceDeck = &source->data.discard.deck;
			break;
		default:
			TraceLog(LOG_ERROR, "ObjectCombine unhandled object type, no action was taken.");
			return false;
	}

	int sourceDeckCount = GetDeckCount(sourceDeck);
	int destDeckRemainingCap = GetDeckMax(destDeck) - GetDeckCount(destDeck);
	if (destDeckRemainingCap >= sourceDeckCount)
	{
		memmove(destDeck->arr + destDeck->count, sourceDeck->arr, sourceDeckCount * sizeof(uint));
		destDeck->count += sourceDeckCount;
		DeleteObject(source);
		return true;
	}
	else if (DeckIsFull(destDeck))
	{
		return false;
	}
	else
	{
		memmove(destDeck->arr + destDeck->count, sourceDeck->arr, destDeckRemainingCap * sizeof(uint));
		destDeck->count += destDeckRemainingCap;
		memmove(sourceDeck->arr, sourceDeck->arr + destDeckRemainingCap, (sourceDeckCount - destDeckRemainingCap) * sizeof(uint));
		sourceDeck->count -= destDeckRemainingCap;
		return false;
	}
}
