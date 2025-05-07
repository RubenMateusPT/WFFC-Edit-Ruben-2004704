#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class InputManager
{

public:
	//Keyboard
	char _keyboardKeys[256];

	bool _upArrow;
	bool _downArrow;
	bool _leftArrow;
	bool _rightArrow;

	bool _leftCtrlKey;
	bool _leftAltKey;

	bool IsKeyPressed(char key);

	bool IsUpArrowPressed();
	bool IsDownArrowPressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();

	bool IsLeftCtrlPressed();
	bool IsLeftAltPressed();

	//Mouse
	DirectX::SimpleMath::Vector2 _mousePosition;
	short _mouseWheelValue;
	bool _mouseButtons[3];

	DirectX::SimpleMath::Vector2 GetMousePosition();
	int GetMouseWheelTurns();
	bool IsMouseButtonPressed(int mouseButton);
};

