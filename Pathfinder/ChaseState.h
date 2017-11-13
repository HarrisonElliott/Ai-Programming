#pragma once

#include "States.h"
#include "IdleState.h"

class ChaseState : public States
{
public:
	ChaseState(StateManager* _manager);
	void Update();

};