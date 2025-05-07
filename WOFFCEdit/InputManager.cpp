#include "InputManager.h"

bool InputManager::IsKeyPressed(char key)
{
	return _keyboardKeys[key];
}

bool InputManager::IsUpArrowPressed()
{
	return _upArrow;
}

bool InputManager::IsDownArrowPressed()
{
	return _downArrow;
}

bool InputManager::IsLeftArrowPressed()
{
	return _leftArrow;
}

bool InputManager::IsRightArrowPressed()
{
	return _rightArrow;
}

bool InputManager::IsLeftCtrlPressed()
{
	return _leftCtrlKey;
}

bool InputManager::IsLeftAltPressed()
{
	return _leftAltKey;
}


DirectX::SimpleMath::Vector2 InputManager::GetMousePosition()
{
	return _mousePosition;
}

int InputManager::GetMouseWheelTurns()
{
	auto mouseTurns = _mouseWheelValue / 120;
	_mouseWheelValue = 0;
	return mouseTurns;
}

bool InputManager::IsMouseButtonPressed(int mouseButton)
{
	return _mouseButtons[mouseButton];
}


