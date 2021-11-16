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

	while (!done && SDL_WaitEvent(&handler.mEvent))
	{
		if (handler.mEvent.type == SDL_QUIT)
		{
			done = true;
		}
		else if (handler.mEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			xStart = handler.mEvent.button.x / 80;
			yStart = handler.mEvent.button.y / 80;
			clickedOn = game.getFieldPos(xStart, yStart);
			if (clickedOn != nullptr)
			{
				if (clickedOn->getTeam() == game.getTurn())
				{
					game.renderPossibleMoves(clickedOn);
				}
			}
		}
		else if (handler.mEvent.type == SDL_MOUSEBUTTONUP)
		{
			if (clickedOn != nullptr)
			{
				if (clickedOn->getTeam() == game.getTurn())
				{
					game.undoRenderPossibleMoves(clickedOn);
				}
			}
			xEnd = handler.mEvent.button.x / 80;
			yEnd = handler.mEvent.button.y / 80;
			if (clickedOn != nullptr)
			{
				if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
					&& (clickedOn->getTeam() == game.getTurn())
					&& (game.isValidMove(xEnd, yEnd, clickedOn)))
				{
					std::vector<Piece::SPieceMovement> list = game.getFieldPos(xStart, yStart)->getPossibleMoves();
					for (const auto& value : list)
					{
						if (value.x == xEnd && value.y == yEnd)
						{
							game.move(clickedOn, std::tuple<int, int, Piece::MoveType>(xEnd, yEnd, value.moveType));
							clickedOn->printPieceStr();
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
