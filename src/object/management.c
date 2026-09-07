#include <string.h>
#include "management.h"
#include "debug.h"
#include "cursor.h"

/// Raw memory block objects are allocated from
Object objects_internal[MAX_OBJECTS] = {0};
/// Ordered list with higher indices being higher off the table.
Object* objects[MAX_OBJECTS] = {0};
int objects_count = 0;
static int next_id = 1;

static const Object* held_object = NULL;
static const Object* picked_object = NULL;

Object* ObjectConstruct(void)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		Object* o = objects_internal + i;
		if (o->id) continue;

		*o = (Object){0};
		o->id = next_id++;

		objects[objects_count] = o;
		objects_count++;

		return o;
	}

	TraceLog(LOG_ERROR, "ObjectConstruct failed, there are no objects left.");
	return NULL;
}

void ObjectFree(const Object* object)
{
	Object* o = ObjectGet(object->id);
	if (!o || o != object)
	{
		TraceLog(LOG_ERROR, "ObjectFree failed when no object matching the specified object's ID was present, or the provided pointer did not match the region in memory!");
		return;
	}

	*o = (Object){0};
}

Object* ObjectGet(int id)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (objects_internal[i].id != id) continue;

		return objects_internal + i;
	}

	TraceLog(LOG_WARNING, "ObjectGet could not find object with ID #%d", id);
	return NULL;
}

Object* ObjectsGet(int* count)
{
	*count = MAX_OBJECTS;
	return objects_internal;
}

Object** ObjectsGetOrdered(int* count)
{
	*count = objects_count;
	return objects;
}

void ObjectsTick(float ft)
{
	// do nothing
}

void ObjectsDraw(void)
{
	bool drawHitboxes = DebugDrawObjectHitboxes();

	for (int i = 0; i < objects_count; i++)
	{
		if (!objects[i]->id) continue;

		if (held_object == objects[i])
		{
			if (CursorHeightGet() == CursorHeightObject)
			{
				CursorDraw();
			}
			ObjectDrawShadowed(objects[i]);
		}
		else if (picked_object == objects[i])
		{
			ObjectDrawHighlight(objects[i], OBJECT_DEFAULT_HIGHLIGHT);
		}
		else
		{
			ObjectDraw(objects[i]);
		}

		if (drawHitboxes)
		{
			DrawRectangleRec(ObjectRect(objects[i]), RED);
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
	for (int i = 0; i < objects_count; i++)
	{
		Object* o = objects[i];
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

// LIST IMPLEMENTATION

static int GetObjectIndex(const Object* object)
{
	for (int i = 0; i < objects_count; i++)
	{
		if (object->id != objects[i]->id) continue;

		return i;
	}

	TraceLog(LOG_ERROR, "GetObjectIndex failed: object not found!");
	return -1;
}

void RemoveObject(Object *object)
{
	int idx = GetObjectIndex(object);
	RemoveObjectAt(idx);
}

Object* RemoveObjectAt(int index)
{
	if (index < 0 || index >= objects_count)
	{
		TraceLog(LOG_WARNING, "RemoveObjectAt failed: invalid index");
		return NULL;
	}

	Object* object = objects[index];

	if (objects_count == 1)
	{
		objects[0] = NULL;
	}
	else if (index == (objects_count - 1))
	{
		objects[index] = NULL;
	}
	else
	{
		size_t sz = (objects_count - 1 - index) * sizeof(Object);
		if (sz <= 0)
		{
			TraceLog(LOG_ERROR, "RemoveObjectAt: Invalid sz");
		}
		memmove(objects + index, objects + index + 1, sz);
		objects[objects_count - 1] = NULL;
	}

	objects_count--;
	return object;
}

void DeleteAllObjects(void)
{
	for (int i = 0; i < MAX_CARDS; i++)
	{
		objects_internal[i] = (Object){0};
		objects[i] = NULL;
	}
	objects_count = 0;
}

Object* MoveObjectToTop(Object *object)
{
	int idx = GetObjectIndex(object);
	if (idx < 0 || idx >= objects_count)
	{
		TraceLog(LOG_ERROR, "MoveObjectToTop failed: the object provided is not in objects array");
		return NULL;
	}

	return MoveObjectAtToTop(idx);
}

Object* MoveObjectAtToTop(int index)
{
	if (index < 0 || index >= objects_count)
	{
		TraceLog(LOG_ERROR, "MoveObjectAtToTop failed: invalid index");
		return NULL;
	}

	Object* value = RemoveObjectAt(index);

	objects[objects_count] = value;
	objects_count++;

	return value;
}
