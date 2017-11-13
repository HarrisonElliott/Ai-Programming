#include "Node.h"

#include <SDL.h>
#include <SDL_Image.h>

void Node::draw(SDL_Renderer* _renderer, SDL_Texture* _image)
{
	SDL_Rect rect = { 0 };
	rect.x = x;
	rect.y = y;
	rect.w = 100;
	rect.h = 100;
	SDL_RenderCopy(_renderer, _image, NULL, &rect);
}

SDL_Rect Node::getRect()
{
	SDL_Rect rectStuff{ x, y, 40, 40 };
	return rectStuff;
}