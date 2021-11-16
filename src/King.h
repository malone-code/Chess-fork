#pragma once

#include "Piece.h"

class King :
	public Piece
{
public:
	King(Team team, const SPosition& pos, SDL_Handler& handler);

	// calculates the possible moves
	void calcPossibleMoves(Piece* field[8][8], bool checkCheck) override;

	// controls whether this piece is checked or not
	void setCheck(Piece* field[8][8], int x, int y);

	// get check
	bool getCheck() { return m_check; };

private:

	// true, if the king is on any of the other teams movelist's
	bool m_check;
};

