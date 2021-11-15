#include "Bishop.h"

#include "Resources.h"

#include <iostream>

Bishop::Bishop(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, BISHOP, pos, handler)
{
	std::string filename = (team == WHITE) ?
		Resources::PieceSprites::kWhiteBishop :
		Resources::PieceSprites::kBlackBishop;
	m_texture = handler.loadImage(filename);
	render();
}

void Bishop::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK BISHOP" << std::endl;
	}
	else
	{
		std::cout << "WHTIE BISHOP" << std::endl;
	}
}

void Bishop::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<SPieceMovement> moves;
	int dx_copy;
	int dy_copy;
	for (int dx = -1; dx <= 1; dx += 2)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			dx_copy = dx;
			dy_copy = dy;
			while (field[m_pos.x + dx_copy][m_pos.y + dy_copy] == nullptr
				&& (m_pos.x + dx_copy >= 0 && m_pos.x + dx_copy <= 7 && m_pos.y + dy_copy >= 0 && m_pos.y + dy_copy <= 7))
			{
				moves = pushMove(moves,
					     { m_pos.x + dx_copy, m_pos.y + dy_copy, Piece::NORMAL },
						 getOwnKing(field),
						 field,
						 checkCheck);
				if (dx_copy < 0)
				{
					dx_copy -= 1;
				}
				else
				{
					dx_copy += 1;
				}
				if (dy_copy < 0)
				{
					dy_copy -= 1;
				}
				else
				{
					dy_copy += 1;
				}
			}
			if (field[m_pos.x + dx_copy][m_pos.y + dy_copy] != nullptr
				&& (m_pos.x + dx_copy >= 0 && m_pos.x + dx_copy <= 7 && m_pos.y + dy_copy >= 0 && m_pos.y + dy_copy <= 7))
			{
				if (field[m_pos.x + dx_copy][m_pos.y + dy_copy]->getTeam() != m_team)
				{
					moves = pushMove(moves,
						{ m_pos.x + dx_copy, m_pos.y + dy_copy, Piece::NORMAL },
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	m_possibleMoves = moves;
}



