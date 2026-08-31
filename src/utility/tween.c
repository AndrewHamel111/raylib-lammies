#include "tween.h"
#include "raymath.h"
#include "utility.h"

typedef enum TweenType
{
	TweenFloat,
	TweenVector2,
} TweenType;

typedef struct Tween
{
	float time;
	float time_original;
	TweenType type;
	TweenStyle style;
	int id;
	union
	{
		struct
		{
			float* target;
			float start;
			float destination;
		} f;
		struct
		{
			Vector2* target;
			Vector2 start;
			Vector2 destination;
		} v2;
	} data;
} Tween;

#define MAX_TWEENS 16
static Tween tweens[MAX_TWEENS] = { 0 };
static TweenStyle tween_style;
static int tween_id = 0;

void SetTweenStyle(TweenStyle style)
{
	tween_style = style;
}

static float TweenGetAmount(Tween* tween)
{
	float t = 1.0f - (tween->time / tween->time_original);

	switch (tween->style)
	{
		case TweenStyleLinear:
			// do nothing
			break;
		case TweenStyleEaseOut:
			t = EASE_OUT_EXP(t);
	}

	return t;
}

static int SetTweenInternal(Tween tween)
{
	for (int i = 0; i < MAX_TWEENS; i++)
	{
		if (tweens[i].time > 0) continue;

		tweens[i] = tween;

		tweens[i].time_original = tween.time;
		tweens[i].style = tween_style;
		tweens[i].id = tween_id++;
		if (tweens[i].type == TweenFloat)
		{
			tweens[i].data.f.start = *tween.data.f.target;
		}
		else if (tweens[i].type == TweenVector2)
		{
			tweens[i].data.v2.start = *tween.data.v2.target;
		}

		return tweens[i].id;
	}

	return -1;
}

int SetTweenf(float* target, float destination, float time)
{
	Tween tween;

	tween.time = time;
	tween.type = TweenFloat;
	tween.data.f.target = target;
	tween.data.f.destination = destination;

	int id = SetTweenInternal(tween);
	if (id < 0)
	{
		TraceLog(LOG_ERROR, "MAX_TWEENS hit, float tween to %.1f completed immediately", destination);
		*target = destination;
		return -1;
	}

	return id;
}

int SetTweenV2(Vector2* target, Vector2 destination, float time)
{
	Tween tween;

	tween.time = time;
	tween.type = TweenVector2;
	tween.data.v2.target = target;
	tween.data.v2.destination = destination;

	int id = SetTweenInternal(tween);
	if (id < 0)
	{
		TraceLog(LOG_ERROR, "MAX_TWEENS hit, Vector2 tween to (%.1f, %.1f) completed immediately", destination.x, destination.y);
		*target = destination;
		return -1;
	}

	return id;
}

void CancelTween(int id)
{
	for (int q = 0; q < MAX_TWEENS; q++)
	{
		if (tweens[q].time < 0 || tweens[q].id != id) continue;

		tweens[q].time = -1;
		return;
	}

	TraceLog(LOG_WARNING, "CancelTween failed - No tween with ID is currently active!");
}

static void FinalizeTween(Tween* tween)
{
	if (tween->type == TweenFloat)
	{
		*(tween->data.f.target) = tween->data.f.destination;
	}
	else if (tween->type == TweenVector2)
	{
		*(tween->data.v2.target) = tween->data.v2.destination;
	}
}

void SkipTween(int id)
{
	for (int q = 0; q < MAX_TWEENS; q++)
	{
		if (tweens[q].time < 0 || tweens[q].id != id) continue;

		tweens[q].time = -1;
		FinalizeTween(tweens + q);
		return;
	}

	TraceLog(LOG_WARNING, "SkipTween failed - No tween with ID is currently active!");
}

static void TickTweenf(Tween* tween, float ft)
{
	tween->time -= ft;

	float t = TweenGetAmount(tween);
	*tween->data.f.target = Lerp(tween->data.f.start, tween->data.f.destination, t);
}

static void TickTweenV2(Tween* tween, float ft)
{
	tween->time -= ft;

	float t = TweenGetAmount(tween);
	*tween->data.v2.target = Vector2Lerp(tween->data.v2.start, tween->data.v2.destination, t);
}

void TickTweens(float time)
{
	for (int q = 0; q < MAX_TWEENS; q++)
	{
		if (tweens[q].time <= 0) continue;

		if (tweens[q].type == TweenFloat)
		{
			TickTweenf(tweens + q, time);
		}
		else if (tweens[q].type == TweenVector2)
		{
			TickTweenV2(tweens + q, time);
		}

		if (tweens[q].time < 0)
		{
			FinalizeTween(tweens + q);
		}
	}
}
