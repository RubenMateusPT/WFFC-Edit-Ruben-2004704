#include "Camera.h"

#include <string>

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

	_zoomSpeed = 2.00;

	_rotationSpeed = 0.25;
}

void Camera::ProcessInput(InputManager* input)
{
	//Get Mouse Information
	_currentMousePostion = input->GetMousePosition();
	_mouseXDifference = _currentMousePostion.x - _lastMousePosition.x;
	_mouseYDifference = _currentMousePostion.y - _lastMousePosition.y;
	_lastMousePosition = _currentMousePostion;

	auto t = L"\nMouse Y: " + std::to_wstring(_currentMousePostion.y);
	OutputDebugStringW(t.c_str());

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
	_mouseWheelTurn = input->GetMouseWheelTurns();
	_zoomIn = _mouseWheelTurn > 0;
	_zoomOut = _mouseWheelTurn < 0;

}

void Camera::Update()
{
	//camera motion is on a plane, so kill the 7 component of the look direction
	DirectX::SimpleMath::Vector3 planarMotionVector = _lookDirection;
	planarMotionVector.y = 0.0;

	if (_rotateHorizontally)
		_orientation.y += _rotationSpeed * _mouseXDifference;

	if (_rotateVertically)
		_orientation.x += _rotationSpeed * -_mouseYDifference;

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
	// Movement 
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

	// Panning
	if (_panVertically)
		_position.y += _panSpeed * _mouseYDifference;

	if (_panHorizontally)
		_position -= _camRight * _panSpeed * _mouseXDifference;


	// Zoom Process
	if (_zoomIn)
		_position += _lookDirection * _zoomSpeed * _mouseWheelTurn;

	if (_zoomOut)
		_position -= _lookDirection * _zoomSpeed * -_mouseWheelTurn;

	//update lookat point
	_lookAt = _position + _lookDirection;
}
