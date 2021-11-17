#pragma once

#include "SDL_Handler.h"

#include <SDL.h>

#include <utility>
#include <vector>

class King;

class Piece
{
public:

	enum Team { BLACK, WHITE };

	enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN };

	enum MoveType { NORMAL, CASTLE, ENPASSANT, NEWPIECE };

	struct SPieceMovement
	{
		int x, y;
		MoveType moveType;
	};

	struct SPosition
	{
		int x, y;
	};

	Piece(Team team, PieceType type, const SPosition& pos, SDL_Handler& handler);

	Piece(const Piece& piece);

	~Piece();

	std::vector<SPieceMovement> getPossibleMoves() const { return mPossibleMoves; }
	Team getTeam() const { return mTeam; }
	void setPosition(SPosition newPos) { mPosition = newPos; }
	const SPosition& getPos() const { return mPosition; }
	PieceType getType() const { return mType; }
	bool hasMoved() const { return mHasMoved; }
	void setHasMoved() { mHasMoved = true; }

	void render();
	void printPieceStr() const;

	virtual void calcPossibleMoves(Piece* field[8][8], bool checkCheck) = 0;

protected:
	// pushes the move, if its allowed.
	// simulates the move, and checks wheter the own king is still checked
	// if king is in check after simulated move, the move is not allowed
	// if checkCheck is true the king simulation will determine whether the move is allowed or not
	// if checkCheck is false, we will just push the move. checkCheck is only false in King's setCheck function,
	// because otherwise it will produce stack overflow (pushMove calls setCheck, setCheck calls pushMove and so on)
	std::vector<SPieceMovement> pushMove(std::vector<SPieceMovement> moveList, SPieceMovement move, King* king, Piece* field[8][8], bool checkCheck);

	King* getOwnKing(Piece* field[8][8]);
	std::string getTeamStr(Team team) const;
	std::string getPieceTypeStr(PieceType pieceType) const;

	Team mTeam;
	PieceType mType;
	SPosition mPosition;
	SDL_Handler& mHandler;
	SDL_Texture* mTexture;
	bool mHasMoved;

	std::vector<SPieceMovement> mPossibleMoves;
};

