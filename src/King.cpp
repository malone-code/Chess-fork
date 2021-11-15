#include "King.h"
#include "Pawn.h"

#include "Resources.h"

#include <iostream>
#include <stdlib.h>

King::King(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, KING, pos, handler), m_check(false)
{
	std::string filename = (team == WHITE) ?
		Resources::PieceSprites::kWhiteKing :
		Resources::PieceSprites::kBlackKing;
	m_texture = handler.loadImage(filename);

	render();
}

void King::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<SPieceMovement> moves;
	bool castles = true;
	bool enemyPlace = false;

	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (m_pos.x + dx >= 0 && m_pos.x + dx <= 7 && m_pos.y + dy >= 0 && m_pos.y + dy <= 7)
			{
				if (field[m_pos.x + dx][m_pos.y + dy] != nullptr)
				{
					if (field[m_pos.x + dx][m_pos.y + dy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
							{m_pos.x + dx, m_pos.y + dy, Piece::NORMAL},
							getOwnKing(field),
							field,
							checkCheck);
					}
				}
				else
				{
					moves = pushMove(moves,
						 {m_pos.x + dx, m_pos.y + dy, Piece::NORMAL},
						 getOwnKing(field),
						 field,
						 checkCheck);
				}
			}
		}
	}

	if (!hasMoved())
	{
		for (int i = 0; i <= 7; i += 7)
		{
			for (int j = 0; j <= 7; j += 7)
			{
				castles = true;
				if (field[i][j] != nullptr)
				{
					if (field[i][j]->getTeam() == m_team && field[i][j]->getType() == ROOK && !field[i][j]->hasMoved())
					{
						int a, b, c;
						if (i == 0)
						{
							a = 1;
							b = 2;
							c = 3;
						}
						else
						{
							a = 5;
							b = 6;
							c = 6;
						}
						if (field[a][j] == nullptr && field[b][j] == nullptr && field[c][j] == nullptr)
						{
							for (int k = 0; k < 8; k++)
							{
								for (int l = 0; l < 8; l++)
								{
									if (field[k][l] != nullptr)
									{
										if (field[k][l]->getTeam() != m_team)
										{
											std::vector<SPieceMovement> notPossible = field[k][l]->getPossibleMoves();
											for (const auto& value : notPossible)
											{
												if (i == 0)
												{
													if ((value.x == 4 && value.y == j)
														|| (value.x == 2 && value.y == j)
														|| (value.x == 3 && value.y == j))
													{
														castles = false;
													}
												}
												else
												{
													if ((value.x == 5 && value.y == j)
														|| (value.x == 6 && value.y == j)
														|| (value.x == 4 && value.y == j))
													{
														castles = false;
													}
												}
											}
										}
									}
								}
							}
							if (castles)
							{
								moves.push_back({i, j, Piece::CASTLE});
							}
						}
					}
				}
			}
		}
	}
	m_possibleMoves = moves;
}

void King::setCheck(Piece* field[8][8], int x, int y)
{
	bool check = false;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() != m_team)
				{
					if (field[i][j]->getType() == KING)
					{
						if (abs(field[i][j]->getPos().x - x) <= 1 && abs(field[i][j]->getPos().y - y) <= 1)
						{
							check = true;
						}

					}
					else if (field[i][j]->getType() == PAWN)
					{
						int dy_pawn;
						if (field[i][j]->getTeam() == WHITE)
						{
							dy_pawn = 1;
						}
						else
						{
							dy_pawn = -1;
						}
						if ((x == field[i][j]->getPos().x + 1 || x == field[i][j]->getPos().x - 1) && y == field[i][j]->getPos().y + dy_pawn)
						{
							check = true;
						}
					}
					else
					{
						field[i][j]->calcPossibleMoves(field, false);
						std::vector<SPieceMovement> notPossible = field[i][j]->getPossibleMoves();
						for (const auto& value : notPossible)
						{
							if (value.x == x && value.y == y)
							{
								check = true;
							}
						}
					}
				}
			}
		}
	}
	
	m_check = check;
}

