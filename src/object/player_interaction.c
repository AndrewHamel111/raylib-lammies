#include "management.h"
#include "raymath.h"
#include "debug.h"
#include "utility.h"
#include "reserve.h"
#include "discard.h"
#include "object/card/animation.h"
#include "cursor.h"
#include "input.h"

static Object* picked_object = NULL;
static ObjectActionList current_action_list = {0};

static Object* held_object = NULL;
static Vector2 held_object_offset = {0};
// TODO: replace with held_object->details.lastValidPosition
static Vector2 held_object_last_position = {0};

typedef enum HoldMode
{
	HoldPick = 0,
	HoldPalm,
} HoldMode;

static HoldMode hold_mode = HoldPick;

static bool trying_pickup = false;

ObjectActionList GetCurrentActionList(void)
{
	return current_action_list;
}

static void PickObject(Object* object)
{
	picked_object = object;

	ObjectSetPicked(object);

	current_action_list = object ? ObjectGetActions(object) : (ObjectActionList){0};
}

static void HoldObjectCustomOffset(Object* object, HoldMode mode, Vector2 offset)
{
	held_object = object;
	held_object_offset = offset;
	ObjectSetHeld(object);
	hold_mode = mode;

	if (object)
	{
		held_object_last_position = object->_position;
	}
}

static void HoldObject(Object* object, HoldMode mode)
{
	Vector2 offset = object ? Vector2Subtract(object->_position, CursorGetPos()) : (Vector2){0};
	HoldObjectCustomOffset(object, mode, offset);
}

void PlayerInteractionUpdate(float ft)
{
	Vector2 curPos = CursorGetPos();

	// new impl, not tested

	PickObject(MousePickObjectExcluding(curPos, held_object));
	Input input = InputGetPressed();

	// TODO: do we need more object flags? i.e. coins, tokens, "the bus" train in dominoes, these would be single click
	bool cardSpecialCase = picked_object && picked_object->type == ObjectCard && input == InputPrimary;

	bool palmRelease = hold_mode == HoldPalm && InputIs(InputSecondary, InputReleased);
	bool pickRelease = hold_mode == HoldPick && InputIs(InputPrimary, InputReleased);
	bool releaseObject = palmRelease || pickRelease;

	bool tryingPickup = CursorGetState() == CursorPalm;
	bool tryingSweep = CursorGetState() == CursorPalmSweep;

	if (picked_object && !picked_object->_locked && !held_object && (tryingPickup || tryingSweep))
	{
		Rectangle objectRec = ObjectRect(picked_object);
		// TODO: add debug option to toggle this "require right side" condition
		bool rightSide = curPos.x > (picked_object->_position.x + (objectRec.width * 0.2f));
		if (rightSide)
		{
			Vector2 offset = curPos;
			offset.x = picked_object->_position.x + objectRec.width - OBJECT_HOLD_OFFSET;
			offset = Vector2Subtract(picked_object->_position, offset);

			HoldObjectCustomOffset(MoveObjectToTop(picked_object), HoldPalm, offset);
			CursorSetHeight(CursorHeightObject);
		}
	}
	else if (cardSpecialCase && !held_object)
	{
		HoldObject(MoveObjectToTop(picked_object), HoldPick);
	}
	else if (held_object && input)
	{
		ObjectInteractionResult result = ObjectHandleInput(held_object, input);
		if (result.type == OIR_ObjectCreatedToHold)
		{
			// This case doesn't really work with my game, I'm assuming I won't need to implement this!
			TraceLog(LOG_WARNING, "OIR_ObjectCreatedToHold case hit in player interaction with held object");
		}
		else if (result.type == OIR_HeldObjectDestroyed)
		{
			HoldObject(NULL, HoldPick);
			CursorSetState(CursorPick);
//			trying_pickup = false;
		}
		else // result.type == OIR_None || result.type == OIR_ObjectCreated
		{
			// do nothing!
		}
	}
	else if (!held_object && picked_object && input)
	{
		ObjectInteractionResult result = ObjectHandleInput(picked_object, input);
		if (result.type == OIR_ObjectCreatedToHold)
		{
			// No need to move, since the object was just created!
			HoldObject(result.object, HoldPick);
		}
		else if (result.type == OIR_ObjectCreated)
		{
			// This case doesn't really work with my game, I'm assuming I won't need to implement this!
			TraceLog(LOG_WARNING, "OIR_ObjectCreated case hit in player interaction with picked object");
		}
		else // result.type == OIR_None
		{
			// do nothing!
		}
	}
	else if (held_object && releaseObject)
	{
		Rectangle heldObjectRec = ObjectRect(held_object);
		heldObjectRec = RectangleInflate(ObjectRect(held_object), OBJECT_PLACEMENT_INFLATE);
		bool hasOverlappingObject = false;

		if (picked_object)
		{
			ObjectCombinationResult result = ObjectCombine(held_object, picked_object);
			if (!(result.type & OCR_SourceDestroyed))
			{
				held_object->_position = held_object_last_position;
			}
			else
			{
				// this object is deleted by ObjectCombine, do nothing
			}
		}
		else
		{
			int objectsCount;
			Object** objects = ObjectsGetOrdered(&objectsCount);
			for (int i = 0; i < objectsCount && !hasOverlappingObject; i++)
			{
				if (!objects[i]->id || objects[i] == held_object) continue;

				if (!CheckCollisionRecs(heldObjectRec, ObjectRect(objects[i]))) continue;

				hasOverlappingObject = true;
			}

			if (hasOverlappingObject)
			{
				// TODO: do we want a card just plucked from an object to return to the object?
				held_object->_position = held_object_last_position;
			}
		}

		HoldObject(NULL, HoldPick);
		CursorSetState(CursorPick);
//		trying_pickup = false;
	}
	else if (picked_object && held_object && tryingSweep && !picked_object->_locked)
	{
		ObjectCombinationResult result = ObjectCombine(picked_object, held_object);
		if (result.type & OCR_NewObject)
		{
			HoldObject(result.object, HoldPalm);
		}
	}
	else
	{
		// Cursor update case
		if (InputIs(InputSecondary, InputPressed))
		{
			if (InputIs(InputPrimary, InputHeld))
			{
				CursorSetState(CursorPalmSweep);
				CursorSetHeight(CursorHeightTable);
			}
			else
			{
				CursorSetState(CursorPalm);
				CursorSetHeight(CursorHeightTable);
			}
		}
		else if (InputIs(InputPrimary, InputPressed))
		{
			if (CursorGetState() == CursorPalm)
			{
				CursorSetState(CursorPalmSweep);
				CursorSetHeight(CursorHeightTable);
			}
		}
		else if (InputIs(InputSecondary, InputReleased))
		{
			CursorSetState(CursorDefault);
			CursorSetHeight(CursorHeightTop);
		}
		else if (InputIs(InputPrimary, InputReleased))
		{
			if (CursorGetState() == CursorPalmSweep)
			{
				CursorSetState(CursorPalm);
				CursorSetHeight(CursorHeightTable);
			}
		}
		else if (picked_object && !held_object && !tryingPickup && !tryingSweep)
		{
			CursorSetState(CursorPick);
			CursorSetHeight(CursorHeightTop);
		}
		else if (!picked_object && !held_object && !tryingPickup && !tryingSweep)
		{
			CursorSetState(CursorDefault);
		}

	}

	if (held_object)
	{
		held_object->_position = Vector2Add(curPos, held_object_offset);
	}
	else if (picked_object)
	{
		Input actionInput = InputGetObjectAction();
		if (actionInput != InputNone && current_action_list.count > 0)
		{
			ObjectHandleAction(picked_object, current_action_list.actions[InputObjectActionToNum(actionInput)]);
		}
	}

	// DEBUG
	if (DebugSpawnFactoryDeck())
	{
		Object* reserve = ObjectCreateReserve(V(128, 128));
		Deck* deck = &reserve->data.reserve.deck;
		InitDeck(deck);
	}
	else if (DebugSpawnShuffledDeck())
	{
		Object* reserve = ObjectCreateReserve(V(128, 128));
		Deck* deck = &reserve->data.reserve.deck;
		InitDeck(deck);
		Shuffle(deck);
	}
	else if (DebugCleanupObjects())
	{
		DeleteAllObjects();
		held_object = NULL;
	}
}
