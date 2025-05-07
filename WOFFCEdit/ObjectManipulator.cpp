#include "ObjectManipulator.h"

ObjectManipulator::Mode ObjectManipulator::GetCurrentSelectedMode()
{
	return _currentMode;
}

std::string ObjectManipulator::GetCurrentSelectedModeName()
{
	switch (_currentMode)
	{
	case None:
		return "None";

	case Translate:
		return "Translate";

	case Rotate:
		return "Rotate";

	case Scale:
		return "Scale";
	}
}

void ObjectManipulator::Initiliazie()
{
	_isBlocked = false;

	_currentMode = None;
	_moveSpeed = 0.1;
	_rotationSpeed = 0.5;
	_scaleSpeed = 0.1;
}

void ObjectManipulator::SetSelectedObject(std::map<int, DisplayObject*>* pickedObjects)
{
	_pickedObjects = pickedObjects;
}

void ObjectManipulator::ProcessInput(InputManager* input)
{
	if (!input->IsMouseButtonPressed(1) && 
		!input->IsMouseButtonPressed(2) &&
		!input->IsLeftAltPressed())
	{
		_isBlocked = false;

		if (input->IsKeyPressed('Q'))
			_currentMode = None;
		else if (input->IsKeyPressed('W'))
			_currentMode = Translate;
		else if (input->IsKeyPressed('E'))
			_currentMode = Rotate;
		else if (input->IsKeyPressed('R'))
			_currentMode = Scale;
	}
	else
	{
		_isBlocked = true;
	}



	//Translate
	_translateForward = input->IsUpArrowPressed();
	_translateBackward = input->IsDownArrowPressed();
	_translateLeft = input->IsLeftArrowPressed();
	_translateRight = input->IsRightArrowPressed();
	_translateUp = input->IsKeyPressed('Z');
	_translateDown = input->IsKeyPressed('X');

	//Rotate
	_pitchUp = input->IsUpArrowPressed();
	_pitchDown = input->IsDownArrowPressed();
	_yawLeft = input->IsLeftArrowPressed();
	_yawRight = input->IsRightArrowPressed();
	_rollForward = input->IsKeyPressed('Z');
	_rollBackward = input->IsKeyPressed('X');

	//Scale
	_scaleForward = input->IsUpArrowPressed();
	_scaleBackward = input->IsDownArrowPressed();
	_scaleLeft = input->IsLeftArrowPressed();
	_scaleRight = input->IsRightArrowPressed();
	_scaleUp = input->IsKeyPressed('Z');
	_scaleDown = input->IsKeyPressed('X');
}

void ObjectManipulator::Update()
{
	if (_isBlocked)
		return;

	if (_pickedObjects->size() == 0)
		return;

	switch (_currentMode)
	{
	case Translate:
		ProcessTranslation();
		break;

	case Rotate:
		ProcessRotation();
		break;

	case Scale:
		ProcessScaling();
		break;
	}

}

void ObjectManipulator::ProcessTranslation()
{
	for (auto pickedObject : *_pickedObjects)
	{
		if (_translateForward)
			pickedObject.second->m_position.z += _moveSpeed;

		if (_translateBackward)
			pickedObject.second->m_position.z -= _moveSpeed;

		if (_translateLeft)
			pickedObject.second->m_position.x -= _moveSpeed;

		if (_translateRight)
			pickedObject.second->m_position.x += _moveSpeed;

		if (_translateUp)
			pickedObject.second->m_position.y += _moveSpeed;

		if (_translateDown)
			pickedObject.second->m_position.y -= _moveSpeed;
	}


}

void ObjectManipulator::ProcessRotation()
{
	for (auto pickedObject : *_pickedObjects)
	{
		if (_rollForward)
			pickedObject.second->m_orientation.z += _rotationSpeed;

		if (_rollBackward)
			pickedObject.second->m_orientation.z -= _rotationSpeed;

		if (_yawLeft)
			pickedObject.second->m_orientation.y -= _rotationSpeed;

		if (_yawRight)
			pickedObject.second->m_orientation.y += _rotationSpeed;

		if (_pitchUp)
			pickedObject.second->m_orientation.x += _rotationSpeed;

		if (_pitchDown)
			pickedObject.second->m_orientation.x -= _rotationSpeed;
	}


}

void ObjectManipulator::ProcessScaling()
{
	for (auto pickedObject : *_pickedObjects)
	{
		if (_scaleForward)
			pickedObject.second->m_scale.z += _scaleSpeed;

		if (_scaleBackward)
			pickedObject.second->m_scale.z -= _scaleSpeed;

		if (_scaleLeft)
			pickedObject.second->m_scale.x -= _scaleSpeed;

		if (_scaleRight)
			pickedObject.second->m_scale.x += _scaleSpeed;

		if (_scaleUp)
			pickedObject.second->m_scale.y += _scaleSpeed;

		if (_scaleDown)
			pickedObject.second->m_scale.y -= _scaleSpeed;

		if (pickedObject.second->m_scale.x < 0)
			pickedObject.second->m_scale.x = 0.02;

		if (pickedObject.second->m_scale.y < 0)
			pickedObject.second->m_scale.y = 0.02;

		if (pickedObject.second->m_scale.z < 0)
			pickedObject.second->m_scale.z = 0.02;
	}

}
