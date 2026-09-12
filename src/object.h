#pragma once

#include "raylib.h"
#include "card/deck.h"

// TODO: be refactored into a general "object animation state" enum
typedef enum CardAnimationState
{
	CardStateDefault = 0,
	CardStateFlippingUpIn,
	CardStateFlippingUpOut,
	CardStateFlippingDownIn,
	CardStateFlippingDownOut,
} CardAnimationState;

typedef enum ObjectType
{
	ObjectReserve,
	ObjectDiscard,
//	ObjectTypePacket
	ObjectCard,
} ObjectType;

typedef struct Object
{
	Vector2 _position;
	int id;
	bool _locked;

	ObjectType type;
	union
	{
		struct
		{
			Deck deck;
		} reserve;
		struct
		{
			Deck deck;
		} discard;
//		struct
//		{
//
//		} packet;
		struct
		{
			int value;
			bool _faceUp;
			float _flipTime;
			// TODO: following general "object animation state" enum refactor, move out of union
			CardAnimationState _animationState;
		} card;
	} data;
} Object;

void ObjectTick(Object* object, float ft);
void ObjectDraw(const Object* object);
void ObjectDrawHighlight(const Object* object, Color highlight);
void ObjectDrawShadowed(const Object* object);

Rectangle ObjectRect(const Object* object);

Object* ObjectCreateReserve(Vector2 position);
Object* ObjectCreateDiscard(Vector2 position);
Object* ObjectCreateCard(Vector2 position, int value);

/// Returns true if the combine deletes the source object, false if the source object should be returned to it's last position
bool ObjectCombine(Object* source, Object* destination);
