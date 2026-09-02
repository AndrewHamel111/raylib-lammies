#pragma once

#include "raylib.h"
#include "card/deck.h"

typedef enum ObjectType
{
	ObjectTypeReserve,
//	ObjectTypeDiscard,
//	ObjectTypePacket
} ObjectType;

typedef struct Object
{
	Vector2 _position;

	ObjectType type;
	union
	{
		struct
		{
			Deck deck;
		} reserve;
//		struct
//		{
//
//		} discard;
//		struct
//		{
//
//		} packet;
	} data;
} Object;
