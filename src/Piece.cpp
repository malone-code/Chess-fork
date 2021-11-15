#include "Piece.h"
#include "King.h"

#include <string>
#include <iostream>

Piece::Piece(Team team, const SPosition& pos, SDL_Handler& handler, PieceType type)
	:m_team(team), m_pos(pos), m_handler(handler), m_texture(NULL), m_hasMoved(false), m_type(type)
{
}

Piece::Piece(const Piece& piece)
	:m_team(piece.m_team), m_pos(piece.m_pos), m_handler(piece.m_handler), m_texture(NULL), m_hasMoved(false), m_type(piece.m_type)
{
}

Piece::~Piece()
{
	SDL_DestroyTexture(m_texture);

	m_handler.undoPieceRender(m_pos.x, m_pos.y);
}

std::vector<Piece::SPieceMovement> Piece::pushMove(std::vector<Piece::SPieceMovement> moveList, Piece::SPieceMovement move, King* king, Piece* field[8][8], bool checkCheck)
{
	if (!checkCheck)
	{
		moveList.push_back(move);
	}
	else
	{
		bool enemyPlace = true;
		king->setCheck(field, king->getPos().x, king->getPos().y);
		Piece* zwisch = &(*field[move.x][move.y]);
		enemyPlace = false;

		if (field[move.x][move.y] != nullptr)
		{
			enemyPlace = true;
			field[move.x][move.y] = nullptr;
		}

		std::swap(field[move.x][move.y], field[m_pos.x][m_pos.y]);
		if (m_type == KING)
		{
			king->setCheck(field, move.x, move.y);
		}
		else
		{
			king->setCheck(field, king->getPos().x, king->getPos().y);
		}
		std::swap(field[move.x][move.y], field[m_pos.x][m_pos.y]);

		if (enemyPlace)
		{
			field[move.x][move.y] = &(*zwisch);
		}
		if (!king->getCheck())
		{
			moveList.push_back(move);
		}
		king->setCheck(field, king->getPos().x, king->getPos().y);
	}
	return moveList;
}

King* Piece::getOwnKing(Piece* field[8][8])
{
	for  (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() == m_team && field[i][j]->getType() == Piece::KING)
				{
					King* ret = static_cast<King*>(field[i][j]);
					return ret;
				}
			}
		}
	}
	return nullptr;
}

void Piece::render()
{
	SDL_Rect src = {0, 0, 60, 60};
	SDL_Rect dest = { m_handler.SCREEN_WIDTH / 8 * m_pos.x - 2,
					  m_handler.SCREEN_HEIGHT / 8 * m_pos.y,
					  m_handler.SCREEN_WIDTH / 8,
					  m_handler.SCREEN_HEIGHT / 8 };
	m_handler.drawRectangle(src, dest, m_texture);
}

void Piece::printPieceStr()
{
	std::cout << getTeamStr(m_team) << " - " << getPieceTypeStr(m_type) << std::endl;
}

std::string Piece::getTeamStr(Team team) {
	switch(team) {
		case WHITE:
			return "WHITE";
		case BLACK:
			return "BLACK";
	}
	return "Unknown Team";
}

std::string Piece::getPieceTypeStr(PieceType pieceType) {
	switch(pieceType) {
		case PAWN:
			return "PAWN";
		case KNIGHT:
			return "KNIGHT";
		case BISHOP:
			return "BISHOP";
		case ROOK:
			return "ROOK";
		case QUEEN:
			return "QUEEN";
		case KING:
			return "KING";
	}
	return "Unknown PieceType";
}

