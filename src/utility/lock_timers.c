#include "lock_timers.h"
#include "constants.h"

typedef struct ObjectLock
{
	Object* object;
	float timeLeft;
} ObjectLock;

static ObjectLock locks[MAX_OBJECT_LOCKS] = {0};

void LockObjectFor(Object* object, float time)
{
	object->_locked = true;
	ObjectLock cardLock = {.object = object, .timeLeft = time};

	for (int i = 0; i < MAX_OBJECT_LOCKS; i++)
	{
		if (locks[i].object != NULL) continue;

		locks[i] = cardLock;
		return;
	}

	TraceLog(LOG_ERROR, "LockObjectFor failed: locks hit MAX_OBJECT_LOCKS");
	object->_locked = false;
}

void TickObjectLocks(float ft)
{
	for (int i = 0; i < MAX_OBJECT_LOCKS; i++)
	{
		if (locks[i].object == NULL) continue;

		locks[i].timeLeft -= ft;
		if (locks[i].timeLeft < 0)
		{
			locks[i].object->_locked = false;
			locks[i].object = NULL;
		}
	}
}
