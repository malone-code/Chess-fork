#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>


class SDL_Handler
{
public:

	SDL_Handler();

	~SDL_Handler();

	// initialize the field
	// returns true, if everything worked
	bool init();

	// renders the source rectangle of the texture to dest rectangle
	void drawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* texture);

	// removes PieceRender
	void undoPieceRender(int x, int y);

	// gets filename, return texture of the jpg file
	SDL_Texture* loadImage(const std::string& filename);

	//renders the background of the field
	void renderBackground();

	// window we'll be rendering to
	SDL_Window* mWindow;
	// Surface contained by the window
	SDL_Surface* mScreenSurface;
	// Renderer
	SDL_Renderer* mRenderer;
	// Event (Mouseclick etc)
	SDL_Event mEvent;

private:
	// cleans up SDL stuff
	void cleanUp();

};

