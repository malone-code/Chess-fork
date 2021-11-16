#pragma once

#include "Piece.h"

class Rook :
	public Piece
{
public:
	Rook(Team team, const SPosition& pos, SDL_Handler& handler);

	~Rook();

	// calculates the possible moves
	void calcPossibleMoves(Piece* field[8][8], bool checkCheck) override;
};



