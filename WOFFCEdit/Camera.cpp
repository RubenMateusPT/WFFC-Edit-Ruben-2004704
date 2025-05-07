#include "Camera.h"
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

	_arcRotationSpeed = 0.005;
	_arcZoomSpeed = 0.01;
	

	// Combo Key Tracker
	_lastComboKeyPressed = None;

	//Object Selection
	_moveToObject = false;
	_distanceOffsetToSelectedObject = 3;
	_maxZoomInDistanceToTarget = 1;
	_imaginaryTargetOffset = 15;
}

void Camera::SetSelectedObject(std::map<int, DisplayObject*>* pickedObjects)
{
	_pickedObjects = pickedObjects;
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
		//Rotation
		_arcRotateVertically = input->IsLeftAltPressed() && input->IsMouseButtonPressed(0) && _mouseYDifference != 0;
		_arcRotateHorizontally = input->IsLeftAltPressed() && input->IsMouseButtonPressed(0) && _mouseXDifference != 0;
		//Zoom
		_arcZoomVertically = input->IsLeftAltPressed() && input->IsMouseButtonPressed(1) && _mouseYDifference != 0;
		_arcZoomHorizontally = input->IsLeftAltPressed() && input->IsMouseButtonPressed(1) && _mouseXDifference != 0;

		//Object Selection
		_moveToObject = input->IsKeyPressed('F') && _pickedObjects->size() > 0;
}

void Camera::Update()
{
	MoveCameraToTarget();

	PanningUpdate();
	MovementUpdate();
	RotationUpdate();
	ZoomUpdate();

	ArcRotationUpdate();
	ArcZoomUpdate();

	ApplyCameraChanges();
}

void Camera::PanningUpdate()
{
	if (_lastComboKeyPressed != MiddleMouse)
		return;

	if (_panVertically)
		_position.y += _panSpeed * _mouseYDifference;

	if (_panHorizontally)
		_position -= _camRight * _panSpeed * _mouseXDifference;

	if (_panVertically || _panHorizontally)
		_hasTarget = false;
}

void Camera::MovementUpdate()
{
	if (_lastComboKeyPressed != RightMouse)
		return;

	// World Movement
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

	if (_moveForward || 
		_moveBackwards ||
		_moveLeft ||
		_moveRight ||
		_moveUp ||
		_moveDown)
		_hasTarget = false;
}

void Camera::RotationUpdate()
{
	if (_lastComboKeyPressed != RightMouse)
		return;

	// Rotation
	if (_rotateVertically)
		_orientation.x += _rotationSpeed * -_mouseYDifference;

	if (_rotateHorizontally)
		_orientation.y += _rotationSpeed * _mouseXDifference;

	//Clamp camera vertical inclination
	if (_orientation.x < -90) // Look at Ground
		_orientation.x = -89.85;
	else if (_orientation.x > 90) // Look at Sky
		_orientation.x = 89.85;

	if (_rotateVertically || _rotateHorizontally)
		_hasTarget = false;
}

void Camera::ArcRotationUpdate()
{
	if (_lastComboKeyPressed != LeftAlt)
		return;

	if (!_hasTarget)
	{
		//if there is no selected target (object) then create a temporary one in the world
		_hasTarget = true;
		_target = _position + _lookDirection * _imaginaryTargetOffset;
	}

	//Get the current direction from camera position to target
	auto targetDirection = _position - _target;
	//Get the camera distance to target (will serve as radius)
	double distanceToTarget = targetDirection.Length();

	//Calculate the cartisian angles in radians
	//the logic comes from the use sphere formulas,
	//where the center is our target, and the camera the points on the edge of the sphere
	double azimuth = atan2(targetDirection.x, targetDirection.z);
	double elevation = asin(targetDirection.y / distanceToTarget);

	if (_arcRotateVertically)
		elevation += _arcRotationSpeed * _mouseYDifference;

	if (_arcRotateHorizontally)
		azimuth += _arcRotationSpeed * -_mouseXDifference;

	if (!_arcRotateVertically && !_arcRotateHorizontally)
		return;

	//Verticly clamp rotation, so that its doent do a turn around
	if (elevation >= DirectX::XM_PIDIV2)
		elevation = 1.56;
	else if (elevation <= -DirectX::XM_PIDIV2)
		elevation = -1.56;

	//Set the camera position based on the sphere formula
	_position.x = _target.x + distanceToTarget * cos(elevation) * sin(azimuth);
	_position.y = _target.y + distanceToTarget * sin(elevation) ;
	_position.z = _target.z + distanceToTarget * cos(elevation) * cos(azimuth);

	//We need to recalute the look direction to face our target
	_lookDirection = _target - _position;
	_lookDirection.Normalize();

	//Convert the look direction from a vector to euler angles in degrees
	_orientation.x = DirectX::XMConvertToDegrees(asin(_lookDirection.y));
	_orientation.y = DirectX::XMConvertToDegrees(atan2(_lookDirection.z, _lookDirection.x));
	_orientation.z = 0;
}

void Camera::ZoomUpdate()
{
	if (_lastComboKeyPressed == LeftAlt)
		return;
	
	if (_zoom)
		_position += _lookDirection * _zoomSpeed * _mouseWheelTurn;

	if (_hasTarget)
	{
		auto distanceToTarget = (_position - _target).Length();
		if (distanceToTarget <= _maxZoomInDistanceToTarget)
			_hasTarget = false;
	}
}

void Camera::ArcZoomUpdate()
{
	if (_lastComboKeyPressed != LeftAlt)
		return;

	auto lastPosition = _position;

	if (_arcZoomVertically)
		_position += _lookDirection * _arcZoomSpeed * _mouseYDifference;

	if (_arcZoomHorizontally)
		_position += _lookDirection * _arcZoomSpeed * _mouseXDifference;

	auto distanceToTarget = (_position - _target).Length();
	if (distanceToTarget <= _maxZoomInDistanceToTarget)
		_position = lastPosition;
}

void Camera::ApplyCameraChanges()
{
	//Convert Euler Angles of Camera to a Look Direction
	auto yRot = DirectX::XMConvertToRadians(_orientation.y);
	auto xRot = DirectX::XMConvertToRadians(_orientation.x);

	_lookDirection.x = cos(yRot) * cos(xRot);
	_lookDirection.y = sin(xRot);
	_lookDirection.z = sin(yRot) * cos(xRot);
	_lookDirection.Normalize();

	//create right vector from look Direction
	_lookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, _camRight);


	//update lookat point
	_lookAt = _position + _lookDirection;
}

void Camera::MoveCameraToTarget()
{
	if (!_moveToObject)
		return;

	if (_pickedObjects->size() == 0)
		return;

	//Execute only Once
	_moveToObject = false;

	//Move camera to object position

	auto targetPos = DirectX::SimpleMath::Vector3(0, 0, 0);
	auto count = 0;
	for (auto po : *_pickedObjects)
	{
		count++;
		targetPos += po.second->m_position;
	}
	targetPos.x /= count;
	targetPos.y /= count;
	targetPos.z /= count;

	_position = targetPos;

	//Offset camera from object by a distance on the Look Direction of the camera
	_position -= _lookDirection * _distanceOffsetToSelectedObject;

	_target = targetPos;
	_hasTarget = true;
}

