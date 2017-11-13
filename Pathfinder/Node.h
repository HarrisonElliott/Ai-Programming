#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <vector>

class Node
{
public:
	Node(int _x, int _y) { x = _x; y = _y; }
	~Node(){};

	int gCost, hCost, fCost; // Values used for A* and other Algorithms
	int x, y; // Node X & Y
	
	void draw(SDL_Renderer* _renderer, SDL_Texture* _image);

	SDL_Rect getRect();
	
	Node* getNeighbour(size_t _id) { return neighbours.at(_id); }
	void addNeighbour(Node* _neighbour) { neighbours.push_back(_neighbour); }
	int getNeighbourCount() { return neighbours.size(); }

	void setObstacle(bool _isObstacle) { isObstacle = _isObstacle; }
	bool getObstacle() { return isObstacle; }

	void setParent(Node* _parent) { parent = _parent; }
	Node* getParent() { return parent; }

	void setHCost(int _f) { cost = _f; }
	int getHCost() { return cost; }

	void solveF() { fCost = gCost + hCost; }

protected:
	int cost;
	std::vector<Node*> neighbours;
	bool isObstacle; // Shows the AI there is an obstacle so it doesnt use that node
	Node* parent;
};