#pragma once

class StateManager;

class States
{
public:
	States(StateManager* _manager);
	virtual ~States();

	virtual void Update() = 0;

	States** self;

protected:
	StateManager* manager;

};