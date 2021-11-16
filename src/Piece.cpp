#include "Piece.h"
#include "King.h"

#include <string>
#include <iostream>

Piece::Piece(Team team, PieceType type, const SPosition& pos, SDL_Handler& handler)
	: mTeam(team), mType(type), mPosition(pos), mHandler(handler), mTexture(NULL), mHasMoved(false)
{
}

Piece::Piece(const Piece& piece)
	: mTeam(piece.mTeam), mType(piece.mType), mPosition(piece.mPosition), mHandler(piece.mHandler), mTexture(NULL), mHasMoved(false)
{
}

Piece::~Piece()
{
	SDL_DestroyTexture(mTexture);

	mHandler.undoPieceRender(mPosition.x, mPosition.y);
}

void Piece::render()
{
	SDL_Rect src = {0, 0, 150, 150};
	SDL_Rect dest = {
		mHandler.SCREEN_WIDTH / 8 * mPosition.x,
		mHandler.SCREEN_HEIGHT / 8 * mPosition.y,
		mHandler.SCREEN_WIDTH / 8,
		mHandler.SCREEN_HEIGHT / 8
	};
	mHandler.drawRectangle(src, dest, mTexture);
}

void Piece::printPieceStr()
{
	std::cout << getTeamStr(mTeam) << " " << getPieceTypeStr(mType) << std::endl;
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

		std::swap(field[move.x][move.y], field[mPosition.x][mPosition.y]);
		if (mType == KING)
		{
			king->setCheck(field, move.x, move.y);
		}
		else
		{
			king->setCheck(field, king->getPos().x, king->getPos().y);
		}
		std::swap(field[move.x][move.y], field[mPosition.x][mPosition.y]);

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
	for  (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() == mTeam && field[i][j]->getType() == Piece::KING)
				{
					King* ret = static_cast<King*>(field[i][j]);
					return ret;
				}
			}
		}
	}
	return nullptr;
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

