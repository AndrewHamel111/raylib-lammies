#pragma once
#include "raylib.h"

typedef enum TweenStyle
{
	TweenStyleLinear = 0,
	TweenStyleEaseOut,
} TweenStyle;

void SetTweenStyle(TweenStyle style);

int SetTweenf(float* target, float destination, float time);
int SetTweenV2(Vector2* target, Vector2 destination, float time);
//void SetTweenV3(Vector3* target, Vector3 destination, float time);

void CancelTween(int id);
void SkipTween(int id);

void TickTweens(float time);
