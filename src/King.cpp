#include "King.h"
#include "Pawn.h"

#include <iostream>
#include <stdlib.h>

namespace {
	const char* const kBlackSprite = "../res/Chess_kdt60.png";
	const char* const kWhiteSprite = "../res/Chess_klt60.png";
}

King::King(Team team, const SPosition& pos, SDL_Handler& handler)
	:Piece(team, pos, handler, KING), m_check(false)
{
	std::string filename = (team == WHITE) ? kWhiteSprite : kBlackSprite;
	m_texture = handler.loadImage(filename);

	render();
}

void King::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK KING" << std::endl;
	}
	else
	{
		std::cout << "WHTIE KING" << std::endl;
	}
}

void King::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple <int, int, Piece::MoveType>> moves;
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
							std::tuple<int, int, Piece::MoveType>(m_pos.x + dx, m_pos.y + dy, Piece::NORMAL),
							getOwnKing(field),
							field,
							checkCheck);
					}
				}
				else
				{
					moves = pushMove(moves,
									 std::tuple<int, int, Piece::MoveType>(m_pos.x + dx, m_pos.y + dy, Piece::NORMAL),
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
											std::vector<std::tuple<int, int, Piece::MoveType>> notPossible = field[k][l]->getPossibleMoves();
											for (const auto& value : notPossible)
											{
												if (i == 0)
												{
													if ((std::get<0>(value) == 4 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 2 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 3 && std::get<1>(value) == j))
													{
														castles = false;
													}
												}
												else
												{
													if ((std::get<0>(value) == 5 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 6 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 4 && std::get<1>(value) == j))
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
								moves.push_back(std::tuple<int, int, Piece::MoveType>(i, j, Piece::CASTLE));
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
						std::vector<std::tuple<int, int, Piece::MoveType>> notPossible = field[i][j]->getPossibleMoves();
						for (const auto& value : notPossible)
						{
							if (std::get<0>(value) == x && std::get<1>(value) == y)
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
