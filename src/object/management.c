#include "management.h"
#include "debug.h"

Object objects[MAX_OBJECTS] = {0};
static int next_id = 1;

static const Object* held_object = NULL;
static const Object* picked_object = NULL;

Object* ObjectConstruct(void)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		Object* o = objects + i;
		if (o->id) continue;

		*o = (Object){0};
		o->id = next_id;
		next_id++;

		return o;
	}

	TraceLog(LOG_ERROR, "ObjectConstruct failed, there are no objects left.");
	return NULL;
}

void ObjectFree(const Object* object)
{
	Object* o = ObjectGet(object->id);
	if (!o)
	{
		TraceLog(LOG_ERROR, "ObjectFree failed when no object matching the specified object's ID was present");
		return;
	}

	o->id = 0;
}

Object* ObjectGet(int id)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (objects[i].id != id) continue;

		return objects + i;
	}

	TraceLog(LOG_WARNING, "ObjectGet could not find object with ID #%d", id);
	return NULL;
}

Object* ObjectsGet(int* count)
{
	*count = MAX_OBJECTS;
	return objects;
}

void ObjectsTick(float ft)
{
	// do nothing
}

void ObjectsDraw(void)
{
	bool drawHitboxes = DebugDrawObjectHitboxes();

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (!objects[i].id) continue;

		if (held_object == objects + i)
		{
			ObjectDrawShadowed(objects + i);
		}
		else if (picked_object == objects + i)
		{
			ObjectDrawHighlight(objects + i, OBJECT_DEFAULT_HIGHLIGHT);
		}
		else
		{
			ObjectDraw(objects + i);
		}

		if (drawHitboxes)
		{
			DrawRectangleRec(ObjectRect(objects + i), RED);
		}
	}
}

Object* MousePickObject(Vector2 mpos)
{
	return MousePickObjectExcluding(mpos, NULL);
}

Object* MousePickObjectExcluding(Vector2 mpos, const Object* excluded)
{
	Object* pickedObject = NULL;
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		Object* o = objects + i;
		if (!o->id || o == excluded) continue;

		Rectangle rect = ObjectRect(o);
		if (!CheckCollisionPointRec(mpos, rect)) continue;

		pickedObject = o;
	}

	return pickedObject;
}

void ObjectSetHeld(const Object* object)
{
	held_object = object;
}

void ObjectSetPicked(const Object* object)
{
	picked_object = object;
}
