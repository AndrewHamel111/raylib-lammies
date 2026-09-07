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
void ObjectDrawHighlight(const Object* object, Color highlight);
void ObjectDrawShadowed(const Object* object);
Rectangle ObjectRect(const Object* object);

Object* ObjectReserveCreate(Vector2 position);
Object* ObjectDiscardCreate(Vector2 position);

/// Returns true if the combine deletes the source object, false if the source object should be returned to it's last position
bool ObjectCombine(Object* source, Object* destination);
