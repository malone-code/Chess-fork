#include "Queen.h"

#include "Resources.h"

#include <iostream>

Queen::Queen(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, QUEEN, pos, handler)
{
	std::string filename = (team == WHITE) ?
		Resources::PieceSprites::kWhiteQueen :
		Resources::PieceSprites::kBlackQueen;
	mTexture = handler.loadImage(filename);
	render();
}

void Queen::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<SPieceMovement> moves;
	int dx_copy;
	int dy_copy;
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			dx_copy = dx;
			dy_copy = dy;
			while (field[mPosition.x + dx_copy][mPosition.y + dy_copy] == nullptr)
			{
				moves = pushMove(moves,
					{mPosition.x + dx_copy, mPosition.y + dy_copy, Piece::NORMAL},
					getOwnKing(field),
					field,
					checkCheck);
				if (dx_copy < 0)
				{
					dx_copy -= 1;
				}
				else if (dx_copy > 0)
				{
					dx_copy += 1;
				}
				if (dy_copy < 0)
				{
					dy_copy -= 1;
				}
				else if (dy_copy > 0)
				{
					dy_copy += 1;
				}
			}
			if (field[mPosition.x + dx_copy][mPosition.y + dy_copy] != nullptr
				&& (mPosition.x + dx_copy >= 0 && mPosition.x + dx_copy <= 7 && mPosition.y + dy_copy >= 0 && mPosition.y + dy_copy <= 7))
			{
				if (field[mPosition.x + dx_copy][mPosition.y + dy_copy]->getTeam() != mTeam)
				{
					moves = pushMove(moves,
						{mPosition.x + dx_copy, mPosition.y + dy_copy, Piece::NORMAL},
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	mPossibleMoves = moves;
}

