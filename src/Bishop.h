#pragma once

#include "Piece.h"
#include "King.h"

class Bishop :
    public Piece
{
public:
    //Constructor
    Bishop(Team team, const SPosition& pos, SDL_Handler& handler);

    // used to debug some stuff
    void sayMyName();

    // calculates the possible moves, 
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};

