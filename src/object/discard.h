#pragma once

#include "object.h"

void DiscardTick(Object* object, float ft);
void DiscardDraw(const Object* object);
Rectangle DiscardRect(Object* object);
ObjectCombinationResult DiscardCombine(Object* source, Object* destination);
bool DiscardFlippable(const Object* object);
void DiscardFlip(Object* object);
void DiscardHandlePickup(Object* object);
void DiscardHandleDrop(Object* object);
ObjectInteractionResult DiscardHandleInput(Object* object, Input input);
void DiscardHandlePicked(Object* object, bool picked);
ObjectActionList DiscardGetActions(const Object* object);
void DiscardHandleAction(Object* object, ObjectAction action);

bool ObjectDiscardFull(const Object* object);
int ObjectDiscardStackHeight(const Object* object);

Object* ObjectDiscardPop(Object* object);
