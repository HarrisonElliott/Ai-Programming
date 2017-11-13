#include "Pathfinders.h"

#include "Node.h"

#include <algorithm>
#include <cmath>
#include <vector>


// Functions for each algorithm which have been implemented

std::vector<Node*> Pathfinders::breadthFirstSearch(Node* _start, Node* _finish)
{
	bool success = false; // When the path isnt found it stays as false, in case of an impossible path

	std::vector<Node*> path; // Used for showing the path 
	std::vector<Node*> open; // Used for having which nodes it can go to
	std::vector<Node*> closed; // Used for having which nodes it can't go to

	open.push_back(_start);

	Node* currentNode; // Setting the node it is at and the next node it is going to
	Node* nextNode; // Setting the next node to go from

	bool isValid = true; // Used to see if the next node is a valid choice

	while (!success && open.size() != 0) // While no path has been found yet
	{
		currentNode = open.front(); 

		for (size_t n = 0; n < currentNode->getNeighbourCount(); n++) // Gets the neighbours for the current node
		{
			nextNode = currentNode->getNeighbour(n); // Setting the next node to a neighbour of the current node
			isValid = !nextNode->getObstacle(); // Making sure the next node is not a wall/obstacle

			for (size_t i = 0; i < open.size(); i++) { // For each node in open
				if (nextNode == open.at(i)) { isValid = false; }
			}

			for (size_t i = 0; i < closed.size(); i++) { // For each node in closed
				if (nextNode == closed.at(i)) { isValid = false; }
			}

			if (isValid) // If isValid is true then run the following
			{
				if (nextNode == _finish) { success = true; } // If the next node is the end goal, success is true, used to exit the while loop for finding the path

				nextNode->setParent(currentNode); // If isValid isn't true then it runs this line and the next, this is setting the Parent of the next node to the one it is at currently
				open.push_back(nextNode); // Put the next node into the open list
			}
		}

		closed.push_back(currentNode); 
		open.erase(open.begin()); 
	}

	if (success) // If the path has been found as it is possible
	{
		currentNode = _finish;

		while (currentNode != _start) // While the node you're at isnt the first node, as we are now going backwards to trace the path
		{
			path.push_back(currentNode);
			currentNode = currentNode->getParent();
		}
	}

	return path; // Return the path it's taken to be shown on the screen
}

std::vector<Node*> Pathfinders::bestFirstSearch(Node* _start, Node* _finish)
{
	// When the path isnt found it stays as false, in case of an impossible path
	bool success = false;

	std::vector<Node*> path; // Used for showing the path 
	std::vector<Node*> open; // Used for having which nodes it can go to
	std::vector<Node*> closed; // Used for having which nodes it can't go to

	open.push_back(_start);

	Node* currentNode; // Setting the node it is at and the next node it is going to
	Node* nextNode; // Setting the next node to go from

	bool isValid = true; // Used to see if the next node is a valid choice

	currentNode = _start;

	_start->hCost = manhattanMethodSolve(_start, _finish); // Working the Heuristic cost of the nodes

	while (!success && open.size() != 0) // While no path has been found yet
	{
		Node* lowest = NULL;

		for (Node* n : open) { 
			if (lowest == NULL) // Written like this so if it's the first time this for loop has run
			{
				lowest = n; // Set the first node as the lowest
			}
			else
			{
				if (n->hCost < lowest->hCost) { lowest = n; } // If the H cost for the next node is lower than the last node, set the new node as the lowest
			}
		}

		if (lowest == _finish) // If the new lowest node is at the goal node
		{
			success = true;
			_finish->setParent(currentNode);
			break;
		}

		currentNode = lowest; // The current node starts as the lowest node

	    /* Remove Moves all elements in the vector matching currentNode to the start and returns the new start, 
		erase then deletes these and sets the next start based on what was passed from remove */
		open.erase(std::remove(open.begin(), open.end(), currentNode), open.end()); 

		closed.push_back(currentNode);

		for (size_t n = 0; n < currentNode->getNeighbourCount(); n++) // for each neighbour of the current node
		{
			Node* neighbour = currentNode->getNeighbour(n);

			if (!neighbour->getObstacle()) // Checking if the neighbour is a wall or not (continues into that block is the neighbour is not a wall)
			{
				bool inClosed = false; // Using this as a check if the neighbour is in the closed list

				for (Node* n : closed) { // For each node in closed
					if (n == neighbour) // If the node is a neighbour
					{
						inClosed = true;
						break;
					}
				}

				if (inClosed) { continue; } // If the node is in the closed array continue on

				bool inOpen = false; // Using this to check if the neighbour is in the open list

				for (Node* n : open) { // For each node in open
					if (n == neighbour) { // If the node is a neighbour
						inOpen = true;
						break;
					}
				}

				if (!inOpen) { open.push_back(neighbour); } // If the neighbour is not in the open list, put the neighbour into the open list

				neighbour->setParent(currentNode); // Setting the parent Node of the neighbour
				neighbour->hCost = manhattanMethodSolve(neighbour, _finish); // Setting the Heuristic cost of the neighbour
			}
		}
	}

	if (success) // If the path has been found as it is possible
	{
		currentNode = _finish;

		while (currentNode != _start) // While the node you're at isnt the first node, as we are now going backwards to trace the path
		{
			path.push_back(currentNode);
			currentNode = currentNode->getParent();
		}
	}

	return path; // Return the path it's taken to be shown on the screen

}

std::vector<Node*> Pathfinders::aStarAlgorithm(Node* _start, Node* _finish)
{
	// When the path isnt found it stays as false, in case of an impossible path
	bool success = false;

	std::vector<Node*> path; // Used for showing the path 
	std::vector<Node*> open; // Used for having which nodes it can go to
	std::vector<Node*> closed; // Used for having which nodes it can't go to

	open.push_back(_start);

	Node* currentNode; // Setting the node it is at and the next node it is going to
	Node* nextNode; // Setting the next node to go from

	bool isValid = true; // Used to see if the next node is a valid choice

	_start->gCost = 0; // Starting the G cost at 0, the G cost is the distance from the start Node
	_start->hCost = manhattanMethodSolve(_start, _finish); // Using the Manhattan Method to solve the Heursitic cost of the start node
	_start->solveF(); // Using a function to solve the F cost to save on lines of code, done in the Node.h file

	currentNode = _start; // Setting the first current node to the Start node

	while (!success && open.size() != 0) // While a path hasnt been found
	{
		Node* lowest = NULL;

		for (Node* n : open) {
			if (lowest == NULL) // Written like this so if it's the first time this for loop has run
			{
				lowest = n; // Set the first node as the lowest
			}
			else
			{
				if (n->fCost < lowest->fCost) { lowest = n; } if (n->hCost < lowest->hCost) { lowest = n; } // If the H cost for the next node is lower than the last node, set the new node as the lowest
			}
		}

		if (lowest == _finish) // If the new lowest node is at the goal node
		{
			success = true;
			_finish->setParent(currentNode);
			break;
		}

		currentNode = lowest; // The current node starts as the lowest node

		open.erase(std::remove(open.begin(), open.end(), currentNode), open.end());
		closed.push_back(currentNode);

		for (size_t n = 0; n < currentNode->getNeighbourCount(); n++) // for each neighbour of the current node
		{
			Node* neighbour = currentNode->getNeighbour(n);

			if (!neighbour->getObstacle()) // Checking if the neighbour is a wall or not (continues into that block is the neighbour is not a wall)
			{
				bool inClosed = false; // Using this as a check if the neighbour is in the closed list

				for (Node* n : closed) { // For each node in closed
					if (n == neighbour) // If the node is a neighbour
					{
						inClosed = true;
						break;
					}
				}

				if (inClosed) { continue; } // If the node is in the closed array continue on

				bool inOpen = false; // Using this to check if the neighbour is in the open list

				for (Node* n : open) { // For each node in open
					if (n == neighbour) { // If the node is a neighbour
						inOpen = true;
						break;
					}
				}

				if (!inOpen) { open.push_back(neighbour); } // If the neighbour is not in the open list, put the neighbour into the open list

				neighbour->setParent(currentNode); // Setting the parent Node of the neighbour
				neighbour->gCost = currentNode->gCost++; // Setting the G cost of the neighbour
				neighbour->hCost = manhattanMethodSolve(neighbour, _finish); // Setting the H cost of the neighbour
				neighbour->solveF(); // Solving the F cost of the neighbour
			}
		}
	}

	if (success) // If the path has been found as it is possible
	{
		currentNode = _finish;

		while (currentNode != _start) // While the node you're at isnt the first node, as we are now going backwards to trace the path
		{
			path.push_back(currentNode);
			currentNode = currentNode->getParent();
		}
	}
	return path; // Return the path it's taken to be shown on the screen
}

// Secondary functions used by the code (Not algorithms)

// Function used for solving the H cost of a Node
int Pathfinders::manhattanMethodSolve(Node* _from, Node* _to)
{
	int dX = std::abs(_from->x - _to->x); // Getting the X axis distance from the current node, to the end node
	int dY = std::abs(_from->y - _to->y); // Getting the Y axis distance from the current node, to the end node

	return dX + dY; // Returning the X and Y values added together which gives the total distance from the current node to end node
}