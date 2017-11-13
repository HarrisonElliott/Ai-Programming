#pragma once

#include "Node.h"

class Player
{
public:
	Player();
	~Player();
	
	void Draw();
	void Update();

	int score;
	Node* position;

};