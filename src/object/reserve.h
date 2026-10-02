#pragma once

#include "object.h"

void ReserveTick(Object* object, float ft);
void ReserveDraw(const Object* object);
Rectangle ReserveRect(Object* object);
ObjectCombinationResult ReserveCombine(Object* source, Object* destination);
bool ReserveFlippable(const Object* object);
void ReserveFlip(Object* object);
void ReserveHandlePickup(Object* object);
void ReserveHandleDrop(Object* object);
ObjectInteractionResult ReserveHandleInput(Object* object, Input input);
void ReserveHandlePicked(Object* object, bool picked);
ObjectActionList ReserveGetActions(const Object* object);
void ReserveHandleAction(Object* object, ObjectAction action);

bool ObjectReserveFull(const Object* object);
int ObjectReserveStackHeight(const Object* object);

Object* ObjectReservePop(Object* object);
