#include <iostream>
#include <Windows.h>

#include "StateManager.h"
#include "IdleState.h"


IdleState::IdleState(StateManager* _manager) : States(_manager)
{
	timer = 300;
}

void IdleState::Update()
{
	if (timer <= 0)
	{
		manager->SetState(new ChaseState(manager));
	}
	else
	{
		timer--;
	}
}