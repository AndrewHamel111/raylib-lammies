#pragma once

#include "object.h"

/// Returns a pointer to an available Object
Object* ObjectConstruct(void);
void ObjectFree(const Object* object);

Object* ObjectGet(int id);
Object* ObjectsGet(int* count);
Object** ObjectsGetOrdered(int* count);

void ObjectsTick(float ft);
void ObjectsDraw(void);

Object* MousePickObject(Vector2 mpos);
Object* MousePickObjectExcluding(Vector2 mpos, const Object* excluded);

void ObjectSetHeld(const Object* object);
void ObjectSetPicked(const Object* object);

// List stuff
void RemoveObject(Object* object);
/// Remove the specified object from it's place on the "table", the object is NOT DELETED until ObjectFree is called.
Object* RemoveObjectAt(int index);
void DeleteAllObjects(void);

Object* MoveObjectToTop(Object* object);
Object* MoveObjectAtToTop(int index);