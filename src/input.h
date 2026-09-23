#pragma once

#include <stdbool.h>

typedef enum Input
{
	InputPrimary = 0,
	InputSecondary,
	InputCount
} Input;

typedef enum InputState
{
	InputNone = 0,
	InputPressed,
	InputHeld,
	InputReleased
} InputState;

void InputUpdate(void);

bool InputIs(Input input, InputState state);
InputState InputGet(Input input);
