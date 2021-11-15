#pragma once

#include "Piece.h"

class Pawn :
    public Piece
{
public:
    // constructor
    Pawn(Team team, const SPosition& pos, SDL_Handler& handler);

    // calculates possible moves
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck) override;

    // direction the pawn moves
    int m_dy;

    // getter m_enPassant
    std::pair<bool, int> getEnPassant() { return m_enPassant; };

    // set m_enPassant
    void setEnPassant(std::pair<bool, int> modifi) { m_enPassant = modifi; };

private:
    // if true, en passant is possible in the int direction
    std::pair<bool, int> m_enPassant;
};

