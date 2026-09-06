#include "object.h"
#include "reserve.h"
#include "utility.h"
#include "debug.h"

void ObjectDraw(const Object* object)
{
	switch (object->type)
	{
		case ObjectTypeReserve:
			ObjectReserveDraw(object);
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
	}

	TraceLog(LOG_WARNING, "ObjectRect unhandled case for object->type %d", object->type);
	return R(0,0,0,0);
}
