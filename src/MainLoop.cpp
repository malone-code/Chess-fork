#include "MainLoop.h"

#include "Game.h"
#include "SDL_events.h"
#include "SDL_Handler.h"

#include <SDL.h>
#include <stdio.h>

void MainLoop::run()
{
	SDL_Handler handler;
	handler.init();
	handler.renderBackground();

	Game game(handler);
	bool done = false;

	int xStart = -1;
	int yStart = -1;
	int xEnd = -1;
	int yEnd = -1;
	Piece* clickedOn = nullptr;

	while (!done && SDL_WaitEvent(&handler.m_event))
	{
		if (handler.m_event.type == SDL_QUIT)
		{
			done = true;
		}
		else if (handler.m_event.type == SDL_MOUSEBUTTONDOWN)
		{
			xStart = handler.m_event.button.x / 80;
			yStart = handler.m_event.button.y / 80;
			clickedOn = game.getFieldPos(xStart, yStart);
			if (clickedOn != nullptr)
			{
				if (clickedOn->getTeam() == game.getTurn())
				{
					game.renderPossibleMoves(clickedOn);
				}
			}
		}
		else if (handler.m_event.type == SDL_MOUSEBUTTONUP)
		{
			if (clickedOn != nullptr)
			{
				if (clickedOn->getTeam() == game.getTurn())
				{
					game.undoRenderPossibleMoves(clickedOn);
				}
			}
			xEnd = handler.m_event.button.x / 80;
			yEnd = handler.m_event.button.y / 80;
			if (clickedOn != nullptr)
			{
				if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
					&& (clickedOn->getTeam() == game.getTurn())
					&& (game.isValidMove(xEnd, yEnd, clickedOn)))
				{
					std::vector<std::tuple<int, int, Piece::MoveType>> list = game.getFieldPos(xStart, yStart)->getPossibleMoves();
					for (const auto& value : list)
					{
						if (std::get<0>(value) == xEnd && std::get<1>(value) == yEnd)
						{
							game.move(clickedOn, std::tuple<int, int, Piece::MoveType>(xEnd, yEnd, std::get<2>(value)));
						}
					}
					xStart = -1;
					yStart = -1;
					yEnd = -1;
					game.calcAllMoves();
					clickedOn = nullptr;
				}
			}
		}
	}
}
