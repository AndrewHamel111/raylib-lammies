#include <string.h>
#include "management.h"
#include "debug.h"
#include "cursor.h"

Object objects[MAX_OBJECTS] = {0};
int objects_count = 0;
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
		objects_count++;
		next_id++;

		return o;
	}

	TraceLog(LOG_ERROR, "ObjectConstruct failed, there are no objects left.");
	return NULL;
}

//void ObjectFree(const Object* object)
//{
//	Object* o = ObjectGet(object->id);
//	if (!o)
//	{
//		TraceLog(LOG_ERROR, "ObjectFree failed when no object matching the specified object's ID was present");
//		return;
//	}
//
//	o->id = 0;
//}

Object* ObjectGet(int id)
{
	for (int i = 0; i < objects_count; i++)
	{
		if (objects[i].id != id) continue;

		return objects + i;
	}

	TraceLog(LOG_WARNING, "ObjectGet could not find object with ID #%d", id);
	return NULL;
}

Object* ObjectsGet(int* count)
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
		if (!objects[i].id) continue;

		if (held_object == objects + i)
		{
			if (CursorHeightGet() == CursorHeightObject)
			{
				CursorDraw();
			}
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
	for (int i = 0; i < objects_count; i++)
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

// LIST IMPLEMENTATION

static int GetObjectIndex(const Object* object)
{
	for (int i = 0; i < objects_count; i++)
	{
		if (object->id != objects[i].id) continue;

		return i;
	}

	TraceLog(LOG_ERROR, "GetObjectIndex failed: object not found!");
	return -1;
}

void DeleteObject(Object *object)
{
	int idx = GetObjectIndex(object);
	DeleteObjectAt(idx);
}

void DeleteObjectAt(int index)
{
	if (index < 0 || index >= MAX_OBJECTS)
	{
		TraceLog(LOG_WARNING, "DeleteObjectAt failed: invalid index");
		return;
	}

	if (objects_count == 1)
	{
		objects[0] = (Object){0};
	}
	else if (index == (objects_count - 1))
	{
		objects[index] = (Object){0};
	}
	else
	{
		size_t sz = (objects_count - 1 - index) * sizeof(Object);
		if (sz <= 0)
		{
			TraceLog(LOG_ERROR, "DeleteObjectAt: Invaild sz");
		}
		memmove(objects + index, objects + index + 1, sz);
		objects[objects_count - 1] = (Object){0};
	}

	objects_count--;
}

void DeleteAllObjects(void)
{
	for (int i = 0; i < MAX_CARDS; i++)
	{
		objects[i] = (Object){0};
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

	Object copy = objects[index];
	DeleteObjectAt(index);

	objects[objects_count] = copy;
	objects_count++;

	return objects + objects_count - 1;
}
