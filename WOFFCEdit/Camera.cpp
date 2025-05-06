#include "Camera.h"

#include <string>
#include <valarray>

void Camera::Initiliazie()
{
	// Camera Track Initial Values
	_position.x = 0.0f;
	_position.y = 3.7f;
	_position.z = -3.5f;

	_orientation.x = 0;
	_orientation.y = 0;
	_orientation.z = 0;

	_lookAt.x = 0.0f;
	_lookAt.y = 0.0f;
	_lookAt.z = 0.0f;

	_lookDirection.x = 0.0f;
	_lookDirection.y = 0.0f;
	_lookDirection.z = 0.0f;

	_camRight.x = 0.0f;
	_camRight.y = 0.0f;
	_camRight.z = 0.0f;

	// Mouse Track Initial Values
	_lastMousePosition = DirectX::SimpleMath::Vector2(0, 0);

	//Camera Settings
	_moveSpeed = 0.30;
	_panSpeed = 0.05;
	_rotationSpeed = 0.25;
	_zoomSpeed = 2.00;

	// Arc Camera
	_arcZoomSpeed = 0.25;

	// Combo Key Tracker
	_lastComboKeyPressed = None;
}

void Camera::ProcessInput(InputManager* input)
{
	// Get Combo Key Information
	// Order Matter, as it give priority
	if (input->IsMouseButtonPressed(2))
		_lastComboKeyPressed = MiddleMouse;
	else if (input->IsLeftAltPressed())
		_lastComboKeyPressed = LeftAlt;
	else if (input->IsMouseButtonPressed(1))
		_lastComboKeyPressed = RightMouse;
	else
		_lastComboKeyPressed = None;

	//Get Mouse Information
	_currentMousePostion = input->GetMousePosition();
	_mouseXDifference = _currentMousePostion.x - _lastMousePosition.x;
	_mouseYDifference = _currentMousePostion.y - _lastMousePosition.y;
	_mouseWheelTurn = input->GetMouseWheelTurns();
	_lastMousePosition = _currentMousePostion;

	// Simple Actions
		// Movement
		_moveForward = input->IsMouseButtonPressed(1) && input->IsKeyPressed('W');
		_moveBackwards = input->IsMouseButtonPressed(1) && input->IsKeyPressed('S');
		_moveLeft = input->IsMouseButtonPressed(1) && input->IsKeyPressed('A');
		_moveRight = input->IsMouseButtonPressed(1) && input->IsKeyPressed('D');
		_moveUp = input->IsMouseButtonPressed(1) && input->IsKeyPressed('Q');
		_moveDown = input->IsMouseButtonPressed(1) && input->IsKeyPressed('E');

		// Panning
		_panVertically = input->IsMouseButtonPressed(2) && _mouseYDifference != 0;
		_panHorizontally = input->IsMouseButtonPressed(2) && _mouseXDifference != 0;

		// Rotation
		_rotateHorizontally = input->IsMouseButtonPressed(1) && _mouseXDifference != 0;
		_rotateVertically = input->IsMouseButtonPressed(1) && _mouseYDifference != 0;

		// Zoom
		_zoom = _mouseWheelTurn != 0;

	// Arcball Camera
		//Zoom
		_arcZoomVertically = input->IsLeftAltPressed() && input->IsMouseButtonPressed(1) && _mouseYDifference != 0;
		_arcZoomHorizontally = input->IsLeftAltPressed() && input->IsMouseButtonPressed(1) && _mouseXDifference != 0;
}

void Camera::Update()
{
	//Action
	if (_lastComboKeyPressed == RightMouse)
	{
		if (_rotateHorizontally)
			_orientation.y += _rotationSpeed * _mouseXDifference;

		if (_rotateVertically)
			_orientation.x += _rotationSpeed * -_mouseYDifference;
	}

	//create look direction from Euler angles in m_camOrientation
	//Clamp camera vertical inclination
	if (_orientation.x < -90)
		_orientation.x = -90;
	else if (_orientation.x > 90)
		_orientation.x = 90;

	auto yRot = _orientation.y * 3.1415 / 180;
	auto xRot = _orientation.x * 3.1415 / 180;

	_lookDirection.x = cos(yRot) * cos(xRot);
	_lookDirection.y = sin(xRot);
	_lookDirection.z = sin(yRot) * cos(xRot);
	_lookDirection.Normalize();

	//create right vector from look Direction
	_lookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, _camRight);

	//process input and update stuff

	// Actions
		// Movement
		if (_lastComboKeyPressed == RightMouse) {
			if (_moveForward)
				_position += _lookDirection * _moveSpeed;

			if (_moveBackwards)
				_position -= _lookDirection * _moveSpeed;

			if (_moveLeft)
				_position -= _camRight * _moveSpeed;

			if (_moveRight)
				_position += _camRight * _moveSpeed;

			if (_moveUp)
				_position.y -= _moveSpeed;

			if (_moveDown)
				_position.y += _moveSpeed;
		}

		// Panning
		if (_lastComboKeyPressed == MiddleMouse) {
			if (_panVertically)
				_position.y += _panSpeed * _mouseYDifference;

			if (_panHorizontally)
				_position -= _camRight * _panSpeed * _mouseXDifference;
		}

		// Zoom
		if (_zoom)
			_position += _lookDirection * _zoomSpeed * _mouseWheelTurn;



	// Arcball
		if (_lastComboKeyPressed == LeftAlt)
		{
			//Zoom
			if (_arcZoomVertically)
				_position += _lookDirection * _arcZoomSpeed * _mouseYDifference;

			if (_arcZoomHorizontally)
				_position += _lookDirection * _arcZoomSpeed * _mouseXDifference;
		}

	//update lookat point
	_lookAt = _position + _lookDirection;
}
