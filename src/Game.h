#pragma once

#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

#include "SDL_Handler.h"

class Game
{
public:
	Game(SDL_Handler& handler);

    ~Game();

	// returns a the Piece in field (row, col)
	Piece* getFieldPos(int row, int col);

    //Moves a piece
    void move(Piece* piece, std::tuple<int, int, Piece::MoveType> move);

    // returns m_turn
    Piece::Team getTurn() { return mTurn; };

    // true, if the move is valid  ;  false if not
    bool isValidMove(int x, int y, Piece* piece);

    // calculates all allowed moves of every piece
    void calcAllMoves();

    // light up the possible Moves
    void renderPossibleMoves(Piece* piece);

    // undos the renderPossibleMoves function
    void undoRenderPossibleMoves(Piece* piece);

private: 
    // disables enPassant for every Piece
    void disableEnPassant();

    // normal move
    void normal(int xStart, int yStart, int xEnd, int yEnd);

    // enpassant move
    void enPassant(int xStart, int yStart, int xEnd, int yEnd);

    // exchange move
    void exchange(int xStart, int yStart, int xEnd, int yEnd);

    // castles move
    void castles(int xStart, int yStart, int xEnd, int yEnd);

    // checks current game state, determines winner or remis
    void gameState();

    // handler
    SDL_Handler& mSDLHandler;

    // Decides whether if its black or whites turn
    Piece::Team mTurn;

    // if true, disable en Passant! if false, dont
    bool mCheckEnPassant;

	// 2D Field array, every Position has got a Piece::Team and a piece
	Piece* mField[8][8];

    // every single piece
    Pawn* pl1;
    Pawn* pl2;
    Pawn* pl3;
    Pawn* pl4;
    Pawn* pl5;
    Pawn* pl6;
    Pawn* pl7;
    Pawn* pl8;
    Pawn* pb1;
    Pawn* pb2;
    Pawn* pb3;
    Pawn* pb4;
    Pawn* pb5;
    Pawn* pb6;
    Pawn* pb7;
    Pawn* pb8;
    Rook* rb1;
    Rook* rb2;
    Rook* rl1;
    Rook* rl2;
    Knight* nb1;
    Knight* nb2;
    Knight* nl1;
    Knight* nl2;
    Bishop* bb1;
    Bishop* bb2;
    Bishop* bl1;
    Bishop* bl2;
    King* kb1;
    King* kl1;
    Queen* qb1; 
    Queen* ql1; 
};

