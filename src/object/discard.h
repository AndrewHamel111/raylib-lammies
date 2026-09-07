#pragma once

#include "object.h"

void ObjectDiscardDraw(const Object* object);
void ObjectDiscardDrawShadowed(const Object* object);
void ObjectDiscardDrawHighlight(const Object* object, Color highlight);

bool ObjectDiscardFull(const Object* object);
int ObjectDiscardStackHeight(const Object* object);

Card* ObjectDiscardPop(Object* object);
