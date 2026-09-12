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

	return object;
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
	float offset = RESERVE_STACK_OFFSET;

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
		DrawTexturePro(tex, src, dest, V(0,0), 0.0f, WHITE);
		dest.y -= offset;
		stackHeight--;
	}
}

void ObjectReserveDraw(const Object* object)
{
	ObjectReserveDrawInternal(object, false, BLANK);
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

	return CLAMPf(object->data.reserve.deck.count, 1, 4);
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
	Object* card = ObjectCreateCard(object->_position, DrawNewCardValue(deck));

	card->_position = object->_position;
	card->data.card._animationState = CardStateDefault;
	card->data.card._faceUp = false;

	// Destroy reserve if the last card is popped
	if (GetDeckCount(deck) == 0)
	{
		RemoveObject(object);
//		ObjectFree(object);
	}

	return card;
}
