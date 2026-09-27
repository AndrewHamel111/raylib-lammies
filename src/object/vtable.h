#pragma once

#include "object.h"

typedef enum Purpose
{
	PurposeTick = 0,
	PurposeDraw,
	PurposeRect,
	PurposeCombine,
	PurposeFlippable,
	PurposeFlip,
	PurposeHandlePickup,
	PurposeHandleDrop,
	PurposeHandleInput,
	PurposeHandlePicked,
	PurposeCount
} Purpose;

typedef void (*funcPtr)(void);
void ObjectRegisterFunc(ObjectType type, Purpose purpose, funcPtr func);
