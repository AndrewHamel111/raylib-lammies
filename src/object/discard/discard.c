#include "object.h"
#include "object/discard.h"
#include "object/management.h"
#include "debug.h"
#include "card/resources.h"
#include "utility.h"
#include "card/list.h"

#include "rlgl.h"

static float card_rotations[] = { -5.0f, 8.0f, -3.0f, 11.0f, -8.0f, 4.0f, -7.0f, 6.0f, -9.0f, 10.0f, -13.0f, 2.0f };

Object* ObjectDiscardCreate(Vector2 position)
{
	Object* object = ObjectConstruct();
	object->type = ObjectTypeDiscard;
	object->_position = position;

	object->data.discard.deck = (Deck){0};

	return object;
}

static void ObjectDiscardDrawInternal(const Object* object, bool shadowed, Color highlight)
{
	if (object->type != ObjectTypeDiscard)
	{
		TraceLog(LOG_ERROR, "ObjectDiscardDrawHighlight called on Object of non-Discard type!");
		return;
	}

	// TODO: implement highlight

	int stackHeight = ObjectDiscardStackHeight(object);
	float offset = DISCARD_STACK_OFFSET;
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
		if (stackHeight == 1 && drawHighlight)
		{
			rlPushMatrix();
			{
				Vector2 cardSize = DebugDrawCardsSmall() ? CARD_SIZE_SMALL : CARD_SIZE;
				rlTranslatef(position.x + cardSize.x / 2, position.y + cardSize.y / 2, 0);
				rlRotatef(card_rotations[i], 0, 0, 1);
				rlTranslatef(-cardSize.x / 2, -cardSize.y / 2, 0);

				Rectangle dest = RectangleInflate(R(0, 0, cardSize.x, cardSize.y), CARD_HIGHLIGHT_EXTENT);
				DrawRectangleRounded(dest, CARD_SHADOW_ROUNDNESS, CARD_SHADOW_SEGMENTS, highlight);
			}
			rlPopMatrix();
		}

		uint value = deck->arr[deck->count - stackHeight];
		CardDrawCustom(position, value % 13, value / 13, card_rotations[i++], 1.0f, BLANK);
		position.y -= offset;
		stackHeight--;
	}
}

void ObjectDiscardDraw(const Object* object)
{
	ObjectDiscardDrawInternal(object, false, BLANK);
}

void ObjectDiscardDrawShadowed(const Object* object)
{
	ObjectDiscardDrawInternal(object, true, BLANK);
}

void ObjectDiscardDrawHighlight(const Object* object, Color highlight)
{
	ObjectDiscardDrawInternal(object, false, highlight);
}

bool ObjectDiscardFull(const Object* object)
{
	if (object->type != ObjectTypeDiscard)
	{
		TraceLog(LOG_TRACE, "ObjectDiscardFull called on Object of non-Discard type!");
		return true;
	}

	const Deck* deck = &object->data.discard.deck;
	return GetDeckCount(deck) == GetDeckMax(deck);
}

int ObjectDiscardStackHeight(const Object* object)
{
	if (object->type != ObjectTypeDiscard)
	{
		TraceLog(LOG_ERROR, "ObjectDiscardStackHeight called on Object of non-Discard type!");
		return true;
	}

	return CLAMPf(object->data.discard.deck.count, 1, 12);
}

Card* ObjectDiscardPop(Object* object)
{
	if (object->type != ObjectTypeDiscard)
	{
		TraceLog(LOG_ERROR, "ObjectDiscardPop called on Object of non-Discard type!");
		return NULL;
	}

	Deck* deck = &(object->data.discard.deck);

	Card card = DrawNewCard(deck);
	card._position = object->_position;
	card._animationState = CardStateDefault;
	card._faceUp = true;

	// Destroy discard if the last card is popped
	if (GetDeckCount(deck) == 0)
	{
		object->id = 0;
	}

	return AddCard(card);
}
