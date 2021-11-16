#include "Knight.h"

#include "Resources.h"

#include <iostream>

Knight::Knight(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, KNIGHT, pos, handler)
{
	std::string filename = (team == WHITE) ?
		Resources::PieceSprites::kWhiteKnight :
		Resources::PieceSprites::kBlackKnight;
	mTexture = handler.loadImage(filename);
	render();
}

void Knight::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<Piece::SPieceMovement> moves;
	
	for (int dx = -2; dx <= 2 ; dx += 4)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			if (mPosition.x + dx >= 0 && mPosition.x + dx <= 7 && mPosition.y + dy >= 0 && mPosition.y + dy <= 7)
			{
				if (field[mPosition.x + dx][mPosition.y + dy] == nullptr)
				{
					moves = pushMove(moves,
									 {mPosition.x + dx, mPosition.y + dy, Piece::NORMAL},
									 getOwnKing(field),
									 field,
									 checkCheck);
				}
				else if (field[mPosition.x + dx][mPosition.y + dy] != nullptr)
				{
					if (field[mPosition.x + dx][mPosition.y + dy]->getTeam() != mTeam)
					{
						moves = pushMove(moves,
										 {mPosition.x + dx, mPosition.y + dy, Piece::NORMAL},
										 getOwnKing(field),
										 field,
										 checkCheck);
					}
				}
			}
		}
	}

	for (int dy = -2; dy <= 2; dy += 4)
	{
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (mPosition.x + dx >= 0 && mPosition.x + dx <= 7 && mPosition.y + dy >= 0 && mPosition.y + dy <= 7)
			{
				if (field[mPosition.x + dx][mPosition.y + dy] == nullptr)
				{
					moves = pushMove(moves,
							{mPosition.x + dx, mPosition.y + dy, Piece::NORMAL},
							getOwnKing(field),
							field,
							checkCheck);
				}
				else if (field[mPosition.x + dx][mPosition.y + dy] != nullptr)
				{
					if (field[mPosition.x + dx][mPosition.y + dy]->getTeam() != mTeam)
					{
						moves = pushMove(moves,
								{mPosition.x + dx, mPosition.y + dy, Piece::NORMAL},
								getOwnKing(field),
								field,
								checkCheck);
					}
				}
			}
		}
	}

	mPossibleMoves = moves;
}
