#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>


class SDL_Handler
{
public:

	// Screen size
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 640;

	SDL_Handler();

	~SDL_Handler();

	// initialize the field
	// returns true, if everything worked
	bool init();

	// renders the source rectangle of the texture to dest rectangle
	void drawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text);

	// removes PieceRender
	void undoPieceRender(int x, int y);

	// gets filename, return texture of the jpg file
	SDL_Texture* loadImage(const std::string& filename);

	//renders the background of the field
	void renderBackground();

	// window we'll be rendering to
	SDL_Window* m_window;
	// Surface contained by the window
	SDL_Surface* m_screenSurface;
	// Renderer
	SDL_Renderer* m_renderer;
	// Event (Mouseclick etc)
	SDL_Event m_event;

private:
	// cleans up SDL stuff
	void cleanUp();

};

