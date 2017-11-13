#pragma once

#include "States.h"
#include "ChaseState.h"

class IdleState : public States
{
private:
	int timer;

public:
	IdleState(StateManager* _manager);
	void Update();

};