#pragma once
#include "DisplayObject.h"
#include "InputManager.h"

class Camera
{
private:
	enum ComboKeys
	{
		None,
		RightMouse,
		MiddleMouse,
		LeftAlt
	};

private:

	//Input
		// Mouse Tracking
			// Mouse Position
			DirectX::SimpleMath::Vector2 _lastMousePosition;
			DirectX::SimpleMath::Vector2 _currentMousePostion;
			float _mouseXDifference;
			float _mouseYDifference;

			// Mouse Wheel
			int _mouseWheelTurn;

		// Combo Keys track
		ComboKeys _lastComboKeyPressed;

	// Camera Tracking
	DirectX::SimpleMath::Vector3 _position;
	DirectX::SimpleMath::Vector3 _orientation; //Euler Angles in Degrees
	DirectX::SimpleMath::Vector3 _lookAt;
	DirectX::SimpleMath::Vector3 _lookDirection; //Camera Facing Direction
	DirectX::SimpleMath::Vector3 _camRight;

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

		// Rotation
		bool _rotateVertically;
		bool _rotateHorizontally;

		bool _arcRotateVertically;
		bool _arcRotateHorizontally;

		// Zoom
		bool _zoom;

		bool _arcZoomVertically;
		bool _arcZoomHorizontally;

	// Camera Setting
	float _moveSpeed;
	float _panSpeed;
	float _rotationSpeed;
	float _zoomSpeed;

	// Camera Arc
	float _arcRotationSpeed;
	float _arcZoomSpeed;

	//Object Selection
	DisplayObject* _selectedObject;
	float _distanceOffsetToSelectedObject;

	bool _hasTarget;
	DirectX::SimpleMath::Vector3 _target;

	bool _moveToObject;

	float _imaginaryTargetOffset;
	float _maxZoomInDistanceToTarget;

public:
	DirectX::SimpleMath::Vector3 GetCameraPosition() { return _position; }
	DirectX::SimpleMath::Vector3 GetCameraLookAt() { return _lookAt; }



public:
	void Initiliazie();
	void SetSelectedObject(DisplayObject* selected);

	void ProcessInput(InputManager* input);
	void Update();

private:
	void PanningUpdate();
	void MovementUpdate();
	void RotationUpdate();
	void ArcRotationUpdate();
	void ZoomUpdate();
	void ArcZoomUpdate();
	void ApplyCameraChanges();

	void MoveCameraToTarget();
};
