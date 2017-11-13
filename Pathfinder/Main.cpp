#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <vector>

#include "ChaseState.h"
#include "IdleState.h"
#include "Node.h"
#include "Pathfinders.h"
#include "StateManager.h"
#include "States.h"

int initialise();
void gameMain(SDL_Renderer* _renderer, SDL_Window* _window);
void setNeighbours(std::vector<Node*> _nodes);

// Setting Window Dimensions
#define SCREENWIDTH 800
#define SCREENHEIGHT 800

// Size of the grid tiles
#define GRID_WIDTH 20
#define GRID_HEIGHT 20

int main(int arc, char* args[]) {
	initialise();
	return 0;
}

// Chose to initalise SDL in a seperate area from main function, out of personal preference
int initialise() {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl; // Initiating SDL
		system("PAUSE");
		return 0;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { 
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl; // Initiating SDL Image
		system("PAUSE");
		return 0;
	}

	SDL_Window* window = SDL_CreateWindow("AI Pathfinding", 50, 50, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN); // Creating the SDL Window
	if (window == NULL) // If there are no variables for the window
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		system("PAUSE");
		return 0;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); // Creating the SDL Renderer
	if (renderer == NULL) // If there are no variables for the renderer
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	gameMain(renderer, window); // Using the renderer and the window I now want to start the "Game" function which hold my main loop
	return 0;
}

// Game main function
void gameMain(SDL_Renderer* _renderer, SDL_Window* _window) {
	
	StateManager manager;

	SDL_Texture* chaser = IMG_LoadTexture(_renderer, "chaser.png");         // Giving a texture for when a chaser tile is needed
	SDL_Texture* chaserPathImg = IMG_LoadTexture(_renderer, "chaserPath.png"); // Giving a texture for when a chaser's path tile is needed
	SDL_Texture* finish = IMG_LoadTexture(_renderer, "finish.png");         // Giving a texture for when a finish tile is needed
	SDL_Texture* hover = IMG_LoadTexture(_renderer, "hover.png");           // Giving a texture for when a hover tile is needed
	SDL_Texture* obstacle = IMG_LoadTexture(_renderer, "obstacle.png");     // Giving a texture for when a obstacle tile is needed
	SDL_Texture* path = IMG_LoadTexture(_renderer, "path.png");             // Giving a texture for when a path tile is needed
	SDL_Texture* start = IMG_LoadTexture(_renderer, "start.png");           // Giving a texture for when a start tile is needed
	SDL_Texture* tile = IMG_LoadTexture(_renderer, "tile.png");             // Giving a texture for when a tile is needed

	std::vector<Node*> nodes; // Creating a vector for the nodes
	for (size_t y = 0; y < (GRID_HEIGHT); y++)
	{
		for (size_t x = 0; x < (GRID_WIDTH); x++)
		{
			Node* node = new Node((SCREENWIDTH / GRID_WIDTH) * x, (SCREENHEIGHT / GRID_HEIGHT) * y);
			nodes.push_back(node);
		}
	}

	SDL_Event event; // Setting the game event
	bool quit = false;

	// Creating nodes start, end and a node for when mouse is hovered over it
	Node* startNode = NULL;
	Node* endNode = NULL;
	Node* hoverNode = NULL;
	Node* chaserNode = NULL;

	// Giving variables for mouse and keyboard
	int mouseX, mouseY;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	std::vector<Node*> nodePath; // Creating a vector for the path the Ai takes
	std::vector<Node*> chaserPath; // Creating a vector for the path the chaser takes

	setNeighbours(nodes); // Working out nodes neighbours
	manager.SetState(new IdleState(&manager)); // Setting the state for the chaser before the game runs

	bool caught = false; // Setting a variable for the chaser
	bool chasing = false; // Setting a variable for chasing
	bool chaserSet = false; // Setting a variable for if there is a chaser on the board

	// Main Game Loop
	while (!quit) {
		SDL_RenderClear(_renderer); // Clearing the renderer

		Uint32 M;
		M = SDL_GetMouseState(&mouseX, &mouseY);

		int Y = mouseY / 40;
		int X = mouseX / 40;

		int N = (Y * GRID_WIDTH) + X;
	
		hoverNode = nodes.at(N);  // Discolour the node highlighted
		
		for (size_t i = 0; i < nodes.size(); i++) // For each node
		{

			if (nodes.at(i)->getObstacle())
			{
				nodes.at(i)->draw(_renderer, obstacle); // If the node is now a wall, add the obstacle
			}
			else
			{
				nodes.at(i)->draw(_renderer, tile);  // If not then leave it as a tile
			}
		}

		if (M & SDL_BUTTON(SDL_BUTTON_LEFT))     { startNode = nodes.at(N); } // Set a start point at this location when prompted with a Left Click
		if (M & SDL_BUTTON(SDL_BUTTON_RIGHT))    { endNode = nodes.at(N); } // Set a end point at this location when prompted with a Right Click
		if (M & SDL_BUTTON(SDL_BUTTON_MIDDLE))   { nodes.at(N)->setObstacle(true); } // Set a Wall at this location when prompted with a Middle Mouse Click
		
		if (keys[SDL_SCANCODE_L]) { chaserNode = nodes.at(N); } // Set a chaser at this location when prompted by 'L' being pressed
		if (keys[SDL_SCANCODE_Q]) { nodes.at(N)->setObstacle(false); } // Used for clearing walls when 'Q' key is pressed
		if (keys[SDL_SCANCODE_M]) { nodePath = Pathfinders::breadthFirstSearch(startNode, endNode); } // Starting the search algorithm 'Breadth-First' when 'M' key is pressed
		if (keys[SDL_SCANCODE_N]) nodePath = Pathfinders::aStarAlgorithm(startNode, endNode); // Starting the search algorithm 'A*' when 'N' key is pressed
		if (keys[SDL_SCANCODE_B]) nodePath = Pathfinders::bestFirstSearch(startNode, endNode); // Starting the search algorithm 'Best-First' when 'B' key is pressed
		if (keys[SDL_SCANCODE_ESCAPE]) quit = true; // Exiting

		for (size_t i = 0; i < nodePath.size(); i++){
			SDL_RenderCopy(_renderer, path, NULL, &nodePath.at(i)->getRect()); // Retrace steps of path taken to objective
		} 
		
		if (hoverNode != NULL)   { SDL_RenderCopy(_renderer, hover, NULL, &hoverNode->getRect()); } // If the mouse is hovered somewhere use this
		if (startNode != NULL)   { SDL_RenderCopy(_renderer, start, NULL, &startNode->getRect()); } // If the start node has been set somewhere, place it
		if (endNode != NULL)     { SDL_RenderCopy(_renderer, finish, NULL, &endNode->getRect()); } // If the end node has been set somewhere, place it
		if (chaserNode != NULL)  { SDL_RenderCopy(_renderer, chaser, NULL, &chaserNode->getRect()); chaserSet = true; } // If the chaser node has been set somewhere render it in and set varible to true;

		if (chaserSet)
		{
			if (keys[SDL_SCANCODE_P]) { chasing = true; }
			chaserPath = Pathfinders::breadthFirstSearch(chaserNode, hoverNode); // The chaser is going to use Breadth-First Search to find a path

			if (chasing)
			{

				for (size_t i = 0; i < chaserPath.size(); i++)
				{
					SDL_RenderCopy(_renderer, chaserPathImg, NULL, &chaserPath.at(i)->getRect()); // Retrace steps of path taken to objective
				}
			}
		}

		SDL_RenderPresent(_renderer); // Update screen with current state

		while (SDL_PollEvent(&event)) {
			quit = event.type == SDL_QUIT; // Exiting
		}
	}


	// Destroy everything when game is finished
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	IMG_Quit();
	SDL_Quit();
}

// Function for setting the neighbours in every direction
void setNeighbours(std::vector<Node*> _nodes)
{
	Node* curr = NULL;

	for (int n = 0; n < _nodes.size(); n++) {
		curr = _nodes.at(n);

		// Direct Neighbours
		if ( n - GRID_WIDTH >= 0 )                                               { curr->addNeighbour(_nodes.at(n - GRID_WIDTH)); }     // NORTH
		if ( n + 1 < _nodes.size() && (n + 1) % GRID_WIDTH != 0 )                { curr->addNeighbour(_nodes.at(n + 1)); }              // EAST
		if ( n + GRID_WIDTH < _nodes.size() )                                    { curr->addNeighbour(_nodes.at(n + GRID_WIDTH)); }     // SOUTH
		if ( n - 1 >= 0 && n % GRID_WIDTH != 0 )                                 { curr->addNeighbour(_nodes.at(n - 1)); }              // WEST

		// Diagonal Neighbours
		if ( n - GRID_WIDTH + 1 < _nodes.size() && (n + 1) % GRID_WIDTH != 0 )   { curr->addNeighbour(_nodes.at(n - GRID_WIDTH + 1)); } // NORTH EAST
		if ( n + GRID_WIDTH + 1 < _nodes.size() && (n + 1) % GRID_WIDTH != 0 )   { curr->addNeighbour(_nodes.at(n + GRID_WIDTH + 1)); } // SOUTH EAST
		if ( n + GRID_WIDTH - 1 < _nodes.size() && n % GRID_WIDTH != 0 )         { curr->addNeighbour(_nodes.at(n + GRID_WIDTH - 1)); } // SOUTH WEST
		if ( n - GRID_WIDTH - 1 >= 0 && n % GRID_WIDTH != 0 )                    { curr->addNeighbour(_nodes.at(n - GRID_WIDTH - 1)); } // NORTH WEST
	}
}