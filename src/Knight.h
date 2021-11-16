#pragma once

#include "Piece.h"

class Knight :
	public Piece
{
public:
	Knight(Team team, const SPosition& pos, SDL_Handler& handler);

	// calculates the possible moves
	void calcPossibleMoves(Piece* field[8][8], bool checkCheck) override;
};

