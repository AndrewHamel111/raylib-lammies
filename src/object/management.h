#pragma once

#include "object.h"

/// Returns a pointer to an available Object
Object* ObjectConstruct(void);
void ObjectFree(const Object* object);

Object* ObjectGet(int id);

void ObjectsTick(float ft);
void ObjectsDraw(void);

Object* MousePickObject(Vector2 mpos);
