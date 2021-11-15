#include "Knight.h"

#include "Resources.h"

#include <iostream>

Knight::Knight(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, KNIGHT, pos, handler)
{
	std::string filename = (team == WHITE) ?
		Resources::PieceSprites::kWhiteKnight :
		Resources::PieceSprites::kBlackKnight;
	m_texture = handler.loadImage(filename);
	render();
}

void Knight::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<Piece::SPieceMovement> moves;
	
	for (int dx = -2; dx <= 2 ; dx += 4)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			if (m_pos.x + dx >= 0 && m_pos.x + dx <= 7 && m_pos.y + dy >= 0 && m_pos.y + dy <= 7)
			{
				if (field[m_pos.x + dx][m_pos.y + dy] == nullptr)
				{
					moves = pushMove(moves,
									 {m_pos.x + dx, m_pos.y + dy, Piece::NORMAL},
									 getOwnKing(field),
									 field,
									 checkCheck);
				}
				else if (field[m_pos.x + dx][m_pos.y + dy] != nullptr)
				{
					if (field[m_pos.x + dx][m_pos.y + dy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
										 {m_pos.x + dx, m_pos.y + dy, Piece::NORMAL},
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
			if (m_pos.x + dx >= 0 && m_pos.x + dx <= 7 && m_pos.y + dy >= 0 && m_pos.y + dy <= 7)
			{
				if (field[m_pos.x + dx][m_pos.y + dy] == nullptr)
				{
					moves = pushMove(moves,
							{m_pos.x + dx, m_pos.y + dy, Piece::NORMAL},
							getOwnKing(field),
							field,
							checkCheck);
				}
				else if (field[m_pos.x + dx][m_pos.y + dy] != nullptr)
				{
					if (field[m_pos.x + dx][m_pos.y + dy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
								{m_pos.x + dx, m_pos.y + dy, Piece::NORMAL},
								getOwnKing(field),
								field,
								checkCheck);
					}
				}
			}
		}
	}

	m_possibleMoves = moves;
}
