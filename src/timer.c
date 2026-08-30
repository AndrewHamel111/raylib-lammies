#include "timer.h"

typedef struct
{
	float time;
	void_callback on_complete;

	int blocking;
} Timer;

#define MAX_TIMERS 4
Timer timers[MAX_TIMERS] = { 0 };

static int SetTimerInternal(float time, void_callback on_complete, int blocking)
{
	for (int i = 0; i < MAX_TIMERS; i++)
	{
		if (timers[i].time > 0) continue;

		timers[i].time = time;
		timers[i].on_complete = on_complete;
		timers[i].blocking = blocking;

		return 1;
	}

	return 0;
}

int SetTimer(float time, void_callback on_complete)
{
	return SetTimerInternal(time, on_complete, 0);
}

int SetTimerBlocking(float time, void_callback on_complete)
{
	return SetTimerInternal(time, on_complete, 1);
}

void TickTimer(float time)
{
	for (int q = 0; q < MAX_TIMERS; q++)
	{
		if (timers[q].time <= 0) continue;

		timers[q].time -= time;

		if (timers[q].time < 0)
		{
			timers[q].on_complete();
			timers[q].on_complete = 0;
		}
	}
}

int HasBlockingTimer(void)
{
	for (int i = 0; i < MAX_TIMERS; i++)
	{
		if (timers[i].time > 0 && timers[i].blocking) return 1;
	}

	return 0;
}