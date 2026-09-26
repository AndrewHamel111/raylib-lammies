#include "vtable.h"

static funcPtr vtable[ObjectTypeCount][PurposeCount] = {0};

void ObjectRegisterFunc(ObjectType type, Purpose purpose, funcPtr func)
{
	if (vtable[type][purpose])
	{
		TraceLog(LOG_ERROR, "ObjectRegisterFunc tried to register a func for type %d purpose %d when one already exists!", type, purpose);
		return;
	}

	vtable[type][purpose] = func;
}

// An interesting idea... for later
// https://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
//#define APPEND_INNER(x, y) x ## y
//#define APPEND(x, y) APPEND_INNER(x, y)
//#define DEF_VOID_SIG(fun, param1) typedef void (* APPEND(fun, _sig))(param1);
//#define DECL_VOID(fun, param1)						\
//DEF_VOID_SIG(fun)									\
//void fun (param1 )             						\
//{													\
//	funcPtr func = vtable[object->type][PurposeTick];	\
//	if (!func) return;								\
//													\
//	((object_tick)func)(object);					\
//}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

typedef void (*object_tick)(Object*, float);
void ObjectTick(Object* object, float ft)
{
	funcPtr func = vtable[object->type][PurposeTick];
	if (!func) return;

	((object_tick)func)(object, ft);
}

typedef void (*object_draw)(const Object*);
void ObjectDraw(const Object* object)
{
	funcPtr func = vtable[object->type][PurposeDraw];
	if (!func) return;

	((object_draw)func)(object);
}

typedef Rectangle (*object_rect)(const Object*);
Rectangle ObjectRect(const Object* object)
{
	funcPtr func = vtable[object->type][PurposeRect];
	if (!func) return (Rectangle){0};

	return ((object_rect)func)(object);
}

typedef bool (*object_combine)(Object*, Object*);
/// Returns true if the combine deletes the source object, false if the source object should be returned to it's last position
bool ObjectCombine(Object* source, Object* destination)
{
	funcPtr func = vtable[source->type][PurposeCombine];
	if (!func) return false;

	return ((object_combine)func)(source, destination);
}

typedef bool (*object_flippable)(const Object*);
bool ObjectFlippable(const Object* object)
{
	funcPtr func = vtable[object->type][PurposeFlippable];
	if (!func) return false;

	return ((object_flippable)func)(object);
}

typedef void (*object_flip)(Object*);
void ObjectFlip(Object* object)
{
	funcPtr func = vtable[object->type][PurposeFlip];
	if (!func) return;

	((object_flip)func)(object);
}

typedef void (*object_handle_pickup)(Object*);
void ObjectHandlePickup(Object* object)
{
	funcPtr func = vtable[object->type][PurposeHandlePickup];
	if (!func) return;

	((object_handle_pickup)func)(object);
}

typedef void (*object_handle_drop)(Object*);
void ObjectHandleDrop(Object* object)
{
	funcPtr func = vtable[object->type][PurposeHandleDrop];
	if (!func) return;

	((object_handle_drop)func)(object);
}

typedef ObjectInteractionResult (*object_handle_input)(Object*, Input);
ObjectInteractionResult ObjectHandleInput(Object* object, Input input)
{
	funcPtr func = vtable[object->type][PurposeHandleInput];
	if (!func) return (ObjectInteractionResult){0};

	return ((object_handle_input)func)(object, input);
}

#pragma clang diagnostic pop
