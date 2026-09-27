#include "input.h"
#include "raylib.h"
#include "constants.h"

static InputState inputs[InputCount];

static InputState GetMouseButtonInternal(int button)
{
	return IsMouseButtonPressed(button) ? InputPressed :
		   IsMouseButtonDown(button) ? InputHeld :
		   IsMouseButtonReleased(button) ? InputReleased :
		   InputNotPressed;
}

void InputUpdate(void)
{
	inputs[InputPrimary] = GetMouseButtonInternal(MOUSE_BUTTON_LEFT);
	inputs[InputSecondary] = GetMouseButtonInternal(MOUSE_BUTTON_RIGHT);
	inputs[InputScrollDown] = GetMouseWheelMoveV().y < -MOUSE_SCROLL_MIN ? InputPressed : InputNotPressed;
	inputs[InputScrollUp] = GetMouseWheelMoveV().y > MOUSE_SCROLL_MIN ? InputPressed : InputNotPressed;
}

bool InputIs(Input input, InputState state)
{
	return inputs[input] == state;
}

InputState InputGet(Input input)
{
	return inputs[input];
}

Input InputGetPressed(void)
{
	return inputs[InputPrimary] == InputPressed ? InputPrimary
		: inputs[InputSecondary] == InputPressed ? InputSecondary
		: inputs[InputScrollDown] == InputPressed ? InputScrollDown
		: inputs[InputScrollUp] == InputPressed ? InputScrollUp
		: InputNone;
}
