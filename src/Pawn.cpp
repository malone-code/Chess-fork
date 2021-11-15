#include "Pawn.h"

#include "Resources.h"

#include <iostream>
#include <list>

Pawn::Pawn(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, PAWN, pos, handler), m_enPassant(std::pair<bool, int>(false, 0))
{
	std::string filename = (team == WHITE) ?
		Resources::PieceSprites::kWhitePawn :
		Resources::PieceSprites::kBlackPawn;
	m_texture = handler.loadImage(filename);

	if (team == BLACK)
	{
		m_dy = -1;
	}
	else
	{
		m_dy = 1;
	}

	render();
}

void Pawn::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<SPieceMovement> moves;

	if (m_pos.y + m_dy == 0 || m_pos.y + m_dy == 7)
	{
		if (field[m_pos.x][m_pos.y + m_dy] == nullptr)
		{
			moves = pushMove(moves,
					{m_pos.x, m_pos.y + m_dy, Piece::NEWPIECE},
					getOwnKing(field),
					field,
					checkCheck);
		}
	}
	else
	{
		if (field[m_pos.x][m_pos.y + m_dy] == nullptr)
		{
			moves = pushMove(moves,
					{m_pos.x, m_pos.y + m_dy, Piece::NORMAL},
					getOwnKing(field),
					field,
					checkCheck);
		}
	}

	if ((m_pos.y + 2 * m_dy >= 0) && (m_pos.y + 2 * m_dy <= 7))
	{
		if (field[m_pos.x][m_pos.y + 2 * m_dy] == nullptr && !hasMoved())
		{
			moves = pushMove(moves,
				{m_pos.x, m_pos.y + 2 * m_dy, Piece::NORMAL},
				getOwnKing(field),
				field,
				checkCheck);
		}
	}

	if (m_pos.x + 1 <= 7)
	{
		if (field[m_pos.x + 1][m_pos.y + m_dy] != nullptr)
		{
			if (field[m_pos.x + 1][m_pos.y + m_dy]->getTeam() != m_team)
			{
				if (m_pos.y + m_dy == 0 || m_pos.y + m_dy == 7)
				{
					moves = pushMove(moves,
						{m_pos.x + 1, m_pos.y + m_dy, Piece::NEWPIECE},
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					moves = pushMove(moves,
						{m_pos.x + 1, m_pos.y + m_dy, Piece::NORMAL},
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}
	if (m_pos.x - 1 >= 0)
	{
		if (field[m_pos.x - 1][m_pos.y + m_dy] != nullptr)
		{
			if (field[m_pos.x - 1][m_pos.y + m_dy]->getTeam() != m_team)
			{
				if (m_pos.y + m_dy == 0 || m_pos.y + m_dy == 7)
				{
					moves = pushMove(moves,
						{m_pos.x - 1, m_pos.y + m_dy, Piece::NEWPIECE},
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					moves = pushMove(moves,
						{m_pos.x - 1, m_pos.y + m_dy, Piece::NORMAL},
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	if (m_enPassant == std::pair<bool, int>(true, -1))
	{
		moves = pushMove(moves,
			{m_pos.x + 1, m_pos.y + m_dy, Piece::ENPASSANT},
			getOwnKing(field),
			field,
			checkCheck);
	}
	if (m_enPassant == std::pair<bool, int>(true, 1))
	{
		moves = pushMove(moves,
			{m_pos.x - 1, m_pos.y + m_dy, Piece::ENPASSANT},
			getOwnKing(field),
			field,
			checkCheck);
	}
	m_possibleMoves = moves;
}






