#include "object/reserve.h"
#include "object.h"
#include "object/management.h"
#include "card/resources.h"
#include "utility.h"
#include "debug.h"
#include "card/list.h"

Object* ObjectReserveCreate(Vector2 position)
{
	Object* object = ObjectConstruct();
	object->type = ObjectTypeReserve;
	object->_position = position;

	object->data.reserve.deck = (Deck){0};

	return object;
}

void ObjectReserveDraw(const Object* object)
{
	ObjectReserveDrawHighlight(object, BLANK);
}

void ObjectReserveDrawHighlight(const Object* object, Color highlight)
{
	if (object->type != ObjectTypeReserve)
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
	Texture2D tex = small ? GetCardBackSmall() : GetCardBackLarge();
	Rectangle src = small ? GetCardSourceSmall() : GetCardSourceLarge();

	stackHeight = ObjectReserveStackHeight(object);

	while (stackHeight > 0)
	{
		DrawTexturePro(tex, src, dest, V(0,0), 0.0f, WHITE);
		dest.y -= offset;
		stackHeight--;
	}
}

bool ObjectReserveFull(const Object* object)
{
	if (object->type != ObjectTypeReserve)
	{
		TraceLog(LOG_TRACE, "ObjectReserveFull called on Object of non-Reserve type!");
		return true;
	}

	return DeckIsFull(&(object->data.reserve.deck));
}

int ObjectReserveStackHeight(const Object* object)
{
	if (object->type != ObjectTypeReserve)
	{
		TraceLog(LOG_ERROR, "ObjectReserveStackHeight called on Object of non-Reserve type!");
		return -1;
	}

	return CLAMPf(object->data.reserve.deck.count, 1, 4);
}

Card* ObjectReservePop(Object* object)
{
	if (object->type != ObjectTypeReserve)
	{
		TraceLog(LOG_ERROR, "ObjectReservePop called on Object of non-Reserve type!");
		return NULL;
	}

	Deck* deck = &(object->data.reserve.deck);

	Card card = DrawNewCard(deck);
	card._position = object->_position;
	card._animationState = CardStateDefault;
	card._faceUp = false;

	// Destroy reserve if the last card is popped
	if (GetDeckCount(deck) == 0)
	{
		object->id = 0;
	}

	return AddCard(card);
}
