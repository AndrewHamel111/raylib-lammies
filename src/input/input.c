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

static InputState GetKeyInternal(int button)
{
	return IsKeyPressed(button) ? InputPressed :
		   IsKeyDown(button) ? InputHeld :
		   IsKeyReleased(button) ? InputReleased :
		   InputNotPressed;
}

void InputUpdate(void)
{
	inputs[InputPrimary] = GetMouseButtonInternal(MOUSE_BUTTON_LEFT);
	inputs[InputSecondary] = GetMouseButtonInternal(MOUSE_BUTTON_RIGHT);
	inputs[InputScrollDown] = GetMouseWheelMoveV().y < -MOUSE_SCROLL_MIN ? InputPressed : InputNotPressed;
	inputs[InputScrollUp] = GetMouseWheelMoveV().y > MOUSE_SCROLL_MIN ? InputPressed : InputNotPressed;

	inputs[InputAction1] = GetKeyInternal(KEY_ONE);
	inputs[InputAction2] = GetKeyInternal(KEY_TWO);
	inputs[InputAction3] = GetKeyInternal(KEY_THREE);
	inputs[InputAction4] = GetKeyInternal(KEY_FOUR);
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

Input InputGetObjectAction(void)
{
	return inputs[InputAction1] == InputPressed ? InputAction1
		: inputs[InputAction2] == InputPressed ? InputAction2
		: inputs[InputAction3] == InputPressed ? InputAction3
		: inputs[InputAction4] == InputPressed ? InputAction4
		: InputNone;
}

int InputObjectActionToNum(Input input)
{
	return (int)(input - InputAction1);
}
