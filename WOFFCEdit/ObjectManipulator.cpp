#include "ObjectManipulator.h"

DisplayObject* ObjectManipulator::GetSelectedObject()
{
	return _selected;
}

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
	_currentMode = None;
	_moveSpeed = 0.1;
	_rotationSpeed = 0.5;
	_scaleSpeed = 0.1;
}

void ObjectManipulator::SetSelectedObject(DisplayObject* selected)
{
	//_currentMode = None;
	_selected = selected;
}

void ObjectManipulator::ProcessInput(InputManager* input)
{
	if (input->IsKeyPressed('Q'))
		_currentMode = None;
	else if (input->IsKeyPressed('W'))
		_currentMode = Translate;
	else if (input->IsKeyPressed('E'))
		_currentMode = Rotate;
	else if (input->IsKeyPressed('R'))
		_currentMode = Scale;

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
	if (_selected == nullptr)
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
	if (_translateForward)
	{
		
	}
		_selected->m_position.z += _moveSpeed;

	if (_translateBackward)
		_selected->m_position.z -= _moveSpeed;

	if (_translateLeft)
		_selected->m_position.x -= _moveSpeed;

	if (_translateRight)
		_selected->m_position.x += _moveSpeed;

	if (_translateUp)
		_selected->m_position.y += _moveSpeed;

	if (_translateDown)
		_selected->m_position.y -= _moveSpeed;
}

void ObjectManipulator::ProcessRotation()
{
	if (_rollForward)
		_selected->m_orientation.z += _rotationSpeed;

	if (_rollBackward)
		_selected->m_orientation.z -= _rotationSpeed;

	if (_yawLeft)
		_selected->m_orientation.y -= _rotationSpeed;

	if (_yawRight)
		_selected->m_orientation.y += _rotationSpeed;

	if (_pitchUp)
		_selected->m_orientation.x += _rotationSpeed;

	if (_pitchDown)
		_selected->m_orientation.x -= _rotationSpeed;
}

void ObjectManipulator::ProcessScaling()
{
	if (_scaleForward)
		_selected->m_scale.z += _scaleSpeed;

	if (_scaleBackward)
		_selected->m_scale.z -= _scaleSpeed;

	if (_scaleLeft)
		_selected->m_scale.x -= _scaleSpeed;

	if (_scaleRight)
		_selected->m_scale.x += _scaleSpeed;

	if (_scaleUp)
		_selected->m_scale.y += _scaleSpeed;

	if (_scaleDown)
		_selected->m_scale.y -= _scaleSpeed;

	if (_selected->m_scale.x < 0.25)
		_selected->m_scale.x = 0.25;

	if (_selected->m_scale.y < 0.25)
		_selected->m_scale.y = 0.25;

	if (_selected->m_scale.z < 0.25)
		_selected->m_scale.z = 0.25;
}
