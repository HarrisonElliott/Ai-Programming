#pragma once

#include "States.h"

class States;

class StateManager
{
public:
	StateManager();
	void SetState(States* _state);
	void Update();

private:
	States* currentState;
};