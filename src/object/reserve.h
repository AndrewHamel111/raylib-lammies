#pragma once

#include "object.h"

void ObjectReserveDraw(const Object* object);
void ObjectReserveDrawShadowed(const Object* object);
void ObjectReserveDrawHighlight(const Object* object, Color highlight);

bool ObjectReserveFull(const Object* object);
int ObjectReserveStackHeight(const Object* object);

Card* ObjectReservePop(Object* object);
