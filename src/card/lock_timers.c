#include "lock_timers.h"
#include "constants.h"

typedef struct CardLock
{
	Card* card;
	float timeLeft;
} CardLock;

static CardLock locks[MAX_CARD_LOCKS] = {0};

void LockCardFor(Card* card, float time)
{
	card->_locked = true;
	CardLock cardLock = {.card = card, .timeLeft = time};

	for (int i = 0; i < MAX_CARD_LOCKS; i++)
	{
		if (locks[i].card != NULL) continue;

		locks[i] = cardLock;
		return;
	}

	TraceLog(LOG_ERROR, "LockCardFor failed: locks hit MAX_CARD_LOCKS");
	card->_locked = false;
}

void TickCardLocks(float ft)
{
	for (int i = 0; i < MAX_CARD_LOCKS; i++)
	{
		if (locks[i].card == NULL) continue;

		locks[i].timeLeft -= ft;
		if (locks[i].timeLeft < 0)
		{
			locks[i].card->_locked = false;
			locks[i].card = NULL;
		}
	}
}
