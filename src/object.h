#pragma once

#include "raylib.h"
#include "card/deck.h"
#include "input.h"

// TODO: be refactored into a general "object animation state" enum
typedef enum CardAnimationState
{
	CardStateDefault = 0,
	CardStateFlippingUpIn,
	CardStateFlippingUpOut,
	CardStateFlippingDownIn,
	CardStateFlippingDownOut,
} CardAnimationState;

typedef enum ObjectType
{
	ObjectCard = 0,
	ObjectReserve,
	ObjectDiscard,
//	ObjectTypePacket
	ObjectTypeCount,
} ObjectType;

typedef enum ObjectAction
{
	OA_None = 0,
	OA_ConvertToReserve,
//	OA_ConvertToMeld,
	OA_Shuffle,
	OA_Lock,
	OA_Unlock,
	OA_Count
} ObjectAction;

typedef struct ObjectActionList
{
	ObjectAction actions[OA_Count];
	int count;
} ObjectActionList;

typedef struct Object
{
	Vector2 _position;
	int id;
	bool _locked;
	bool _held;

	ObjectType type;
	union
	{
		struct
		{
			Deck deck;
			bool _faceUp;
			int scrollIndex;
		} reserve;
		struct
		{
			Deck deck;
			bool _faceUp;
			int scrollIndex;
		} discard;
//		struct
//		{
//
//		} packet;
		struct
		{
			int value;
			bool _faceUp;
			float _flipTime;
			// TODO: following general "object animation state" enum refactor, move out of union
			CardAnimationState _animationState;
		} card;
	} data;
} Object;

typedef enum ObjectInteractionResultType
{
	OIR_None = 0,
	OIR_ObjectCreated,
	OIR_ObjectCreatedToHold,
	OIR_HeldObjectDestroyed,
} ObjectInteractionResultType;

typedef struct ObjectInteractionResult
{
	ObjectInteractionResultType type;
	Object* object;
} ObjectInteractionResult;

typedef enum ObjectCombinationResultType
{
	OCR_Failed = 0,
	OCR_Success = 1,
	OCR_NewObject = 2,
	OCR_SourceDestroyed = 4,
} ObjectCombinationResultType;

typedef struct ObjectCombinationResult
{
	ObjectCombinationResultType type;
	Object* object;
} ObjectCombinationResult;

// Object interface functions, implemented in vtable.c
void ObjectTick(Object* object, float ft);
void ObjectDraw(const Object* object);
Rectangle ObjectRect(const Object* object);
/// Returns true if the combine deletes the source object, false if the source object should be returned to it's last position
ObjectCombinationResult ObjectCombine(Object* source, Object* destination);
bool ObjectFlippable(const Object* object);
void ObjectFlip(Object* object);
void ObjectHandlePickup(Object* object);
void ObjectHandleDrop(Object* object);
ObjectInteractionResult ObjectHandleInput(Object* object, Input input);
void ObjectHandlePicked(Object* object, bool picked);
ObjectActionList ObjectGetActions(const Object* object);
void ObjectHandleAction(Object* object, ObjectAction action);

void ObjectSetDrawHighlight(Color highlight);
void ObjectSetDrawShadowed(void);

Color ObjectGetDrawHighlight(void);
bool ObjectGetDrawShadowed(void);

// TODO: pending deletion
Object* ObjectCreateReserve(Vector2 position);
Object* ObjectCreateDiscard(Vector2 position);
Object* ObjectCreateCard(Vector2 position, int value);

const char* ObjectActionName(ObjectAction action);
