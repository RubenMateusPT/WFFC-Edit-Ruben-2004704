#pragma once
#include "InputManager.h"

class Camera
{
private:

	// Camera Tracking
	DirectX::SimpleMath::Vector3 _position;
	DirectX::SimpleMath::Vector3 _orientation;
	DirectX::SimpleMath::Vector3 _lookAt;
	DirectX::SimpleMath::Vector3 _lookDirection;
	DirectX::SimpleMath::Vector3 _camRight;

	// Mouse Tracking
		// Mouse Position
		DirectX::SimpleMath::Vector2 _lastMousePosition;
		DirectX::SimpleMath::Vector2 _currentMousePostion;
		float _mouseXDifference;
		float _mouseYDifference;

		// Mouse Wheel
		int _mouseWheelTurn;

	// Camera Actions
		// Movement
		bool _moveForward;
		bool _moveBackwards;
		bool _moveLeft;
		bool _moveRight;
		bool _moveUp;
		bool _moveDown;

		// Panning
		bool _panVertically;
		bool _panHorizontally;




	bool _zoomIn;
	bool _zoomOut;

	bool _rotateHorizontally;
	bool _rotateVertically;



	// Camera Settings
	float _moveSpeed;
	float _panSpeed;

	float _zoomSpeed;

	float _rotationSpeed;

public:
	DirectX::SimpleMath::Vector3 GetCameraPosition() { return _position; }
	DirectX::SimpleMath::Vector3 GetCameraLookAt() { return _lookAt; }

public:
	void Initiliazie();

	void ProcessInput(InputManager* input);
	void Update();


	bool t() { return _zoomIn; };
};
