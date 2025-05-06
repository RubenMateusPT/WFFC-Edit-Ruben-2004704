#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class InputManager
{

public:
	//Keyboard
	char _keyboardKeys[256];
	bool IsKeyPressed(char key);

	//Mouse
	DirectX::SimpleMath::Vector2 _mousePosition;
	short _mouseWheelValue;
	bool _mouseButtons[3];

	DirectX::SimpleMath::Vector2 GetMousePosition();
	int GetMouseWheelTurns();
	bool IsMouseButtonPressed(int mouseButton);
};

