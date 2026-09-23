#include "input.h"
#include "raylib.h"

static InputState inputs[InputCount];

static InputState GetMouseButtonInternal(int button)
{
	return IsMouseButtonPressed(button) ? InputPressed :
		   IsMouseButtonDown(button) ? InputHeld :
		   IsMouseButtonReleased(button) ? InputReleased :
		   InputNone;
}

void InputUpdate(void)
{
	inputs[InputPrimary] = GetMouseButtonInternal(MOUSE_BUTTON_LEFT);
	inputs[InputSecondary] = GetMouseButtonInternal(MOUSE_BUTTON_RIGHT);
}

bool InputIs(Input input, InputState state)
{
	return inputs[input] == state;
}

InputState InputGet(Input input)
{
	return inputs[input];
}
