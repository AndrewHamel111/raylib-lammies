#pragma once

#include "raylib.h"
#include "card/deck.h"

typedef enum ObjectType
{
	ObjectTypeReserve,
	ObjectTypeDiscard,
//	ObjectTypePacket
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
	} data;
} Object;

void ObjectDraw(const Object* object);
void ObjectDrawShadowed(const Object* object);
Rectangle ObjectRect(const Object* object);

Object* ObjectReserveCreate(Vector2 position);
Object* ObjectDiscardCreate(Vector2 position);
