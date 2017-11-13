#pragma once

#include <vector>

#include "Node.h"

//Static class - Pathfinders
class Pathfinders
{
public:
	static std::vector<Node*> breadthFirstSearch(Node* _start, Node* _finish);
	static std::vector<Node*> bestFirstSearch(Node* _start, Node* _finish);
	static std::vector<Node*> aStarAlgorithm(Node* _start, Node* _finish);

	static int manhattanMethodSolve(Node* _from, Node* _to);
private:
	Pathfinders();
	~Pathfinders();
};