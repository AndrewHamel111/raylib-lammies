#pragma once

#include <stdbool.h>

typedef enum Input
{
	InputNone = 0,
	InputPrimary = 1,
	InputSecondary,
	InputScrollUp,
	InputScrollDown,
	InputAction1,
	InputAction2,
	InputAction3,
	InputAction4,
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
Input InputGetObjectAction(void);
int InputObjectActionToNum(Input input);
