#pragma once
#include "DisplayObject.h"
#include "InputManager.h"


class ObjectManipulator
{
public:
	enum Mode
	{
		None,
		Translate,
		Rotate,
		Scale
	};

private:
	//Inputs
		//Translate
		bool _translateForward;
		bool _translateBackward;
		bool _translateLeft;
		bool _translateRight;
		bool _translateUp;
		bool _translateDown;

		//Rotate
		bool _rollForward;
		bool _rollBackward;
		bool _yawLeft;
		bool _yawRight;
		bool _pitchUp;
		bool _pitchDown;

		//Scale
		bool _scaleForward;
		bool _scaleBackward;
		bool _scaleLeft;
		bool _scaleRight;
		bool _scaleUp;
		bool _scaleDown;

	//Settings
	Mode _currentMode;
	DisplayObject* _selected;
	float _moveSpeed;
	float _rotationSpeed;
	float _scaleSpeed;

public:
	DisplayObject* GetSelectedObject();
	Mode GetCurrentSelectedMode();
	std::string GetCurrentSelectedModeName();

public:
	void Initiliazie();
	void SetSelectedObject(DisplayObject* selected);

	void ProcessInput(InputManager* input);
	void Update();

private:
	void ProcessTranslation();
	void ProcessRotation();
	void ProcessScaling();
};

