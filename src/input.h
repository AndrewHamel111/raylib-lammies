#pragma once

#include <stdbool.h>

typedef enum Input
{
	InputNone = 0,
	InputPrimary = 1,
	InputSecondary,
	InputScrollUp,
	InputScrollDown,
	InputCount
} Input;

typedef enum InputState
{
	InputNotPressed = 0,
	InputPressed,
	InputHeld,
	InputReleased
} InputState;

void InputUpdate(void);

bool InputIs(Input input, InputState state);
InputState InputGet(Input input);
Input InputGetPressed(void);
