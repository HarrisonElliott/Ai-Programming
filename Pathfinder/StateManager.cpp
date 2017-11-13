#include "StateManager.h"

#include <iostream>

StateManager::StateManager()
{
	currentState = NULL;
}

void StateManager::SetState(States* _state)
{
	if (currentState != NULL)
	{
		delete currentState;
	}

	currentState = _state;
}

void StateManager::Update()
{
	if (currentState != NULL)
	{
		currentState->Update();
	}
}