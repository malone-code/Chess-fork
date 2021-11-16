#include "SDL_Handler.h"

#include <iostream>

SDL_Handler::SDL_Handler()
	: mWindow(nullptr)
	, mScreenSurface(nullptr)
	, mRenderer(nullptr)
{
	// Set better scale quality
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

SDL_Handler::~SDL_Handler()
{
	cleanUp();
}

SDL_Texture* SDL_Handler::loadImage(const std::string& filename)
{
	SDL_Texture* texture = nullptr;

	if (SDL_Surface* loadedImage = IMG_Load(filename.c_str()))
	{
		texture = SDL_CreateTextureFromSurface(mRenderer, loadedImage);
	}
	else
	{
		std::cout << "Couldnt load " << filename << std::endl;
	}

	return texture;
}

void SDL_Handler::renderBackground()
{
	bool white = true;
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (white)
			{
				SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(mRenderer, 155, 103, 60, 255);
			}
			white = !white;
			SDL_Rect rectangle = { i * SCREEN_WIDTH / 8,
								  j * SCREEN_HEIGHT / 8,
								  SCREEN_WIDTH / 8,
								  SCREEN_HEIGHT / 8 };
			SDL_RenderFillRect(mRenderer, &rectangle);
		}
		white = !white;
	}
}

void SDL_Handler::undoPieceRender(int x, int y)
{
	if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1))
	{
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(mRenderer, 155, 103, 60, 255);
	}
	SDL_Rect rectangle = {
		x * SCREEN_WIDTH / 8,
		y * SCREEN_HEIGHT / 8,
		SCREEN_WIDTH / 8,
		SCREEN_HEIGHT / 8
	};
	SDL_RenderFillRect(mRenderer, &rectangle);
}

void SDL_Handler::cleanUp()
{
	SDL_FreeSurface(mScreenSurface);
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void SDL_Handler::drawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* texture)
{
	if (texture)
	{
		SDL_RenderCopy(mRenderer, texture, &source, &dest);
		SDL_RenderPresent(mRenderer);

		SDL_UpdateWindowSurface(mWindow);
	}
	else
	{
		std::cout << "DrawRectangle: texture was nullptr" << std::endl;
	}
}

bool SDL_Handler::init()
{
	mWindow = NULL;
	mScreenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		//Create window
		mWindow = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mWindow != NULL)
		{
			mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
		}
		else
		{
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		}
	}
	else
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		cleanUp();
		return false;
	}
	return true;
}

