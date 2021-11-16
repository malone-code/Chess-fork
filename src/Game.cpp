#include "Game.h"

#include "Resources.h"

#include <iostream>

Game::Game(SDL_Handler& handler)
	: mSDLHandler(handler)
	, mTurn(Piece::WHITE)
	, mCheckEnPassant(true)
	, pl1(new Pawn(Piece::WHITE, {0, 1}, handler))
	, pl2(new Pawn(Piece::WHITE, {1, 1}, handler))
	, pl3(new Pawn(Piece::WHITE, {2, 1}, handler))
	, pl4(new Pawn(Piece::WHITE, {3, 1}, handler))
	, pl5(new Pawn(Piece::WHITE, {4, 1}, handler))
	, pl6(new Pawn(Piece::WHITE, {5, 1}, handler))
	, pl7(new Pawn(Piece::WHITE, {6, 1}, handler))
	, pl8(new Pawn(Piece::WHITE, {7, 1}, handler))
	, pb1(new Pawn(Piece::BLACK, {0, 6}, handler))
	, pb2(new Pawn(Piece::BLACK, {1, 6}, handler))
	, pb3(new Pawn(Piece::BLACK, {2, 6}, handler))
	, pb4(new Pawn(Piece::BLACK, {3, 6}, handler))
	, pb5(new Pawn(Piece::BLACK, {4, 6}, handler))
	, pb6(new Pawn(Piece::BLACK, {5, 6}, handler))
	, pb7(new Pawn(Piece::BLACK, {6, 6}, handler))
	, pb8(new Pawn(Piece::BLACK, {7, 6}, handler))
	, rb1(new Rook(Piece::BLACK, {0, 7}, handler))
	, rb2(new Rook(Piece::BLACK, {7, 7}, handler))
	, rl1(new Rook(Piece::WHITE, {0, 0}, handler))
	, rl2(new Rook(Piece::WHITE, {7, 0}, handler))
	, nb1(new Knight(Piece::BLACK, {1, 7}, handler))
	, nb2(new Knight(Piece::BLACK, {6, 7}, handler))
	, nl1(new Knight(Piece::WHITE, {1, 0}, handler))
	, nl2(new Knight(Piece::WHITE, {6, 0}, handler))
	, bb1(new Bishop(Piece::BLACK, {2, 7}, handler))
	, bb2(new Bishop(Piece::BLACK, {5, 7}, handler))
	, bl1(new Bishop(Piece::WHITE, {2, 0}, handler))
	, bl2(new Bishop(Piece::WHITE, {5, 0}, handler))
	, kb1(new King(Piece::BLACK, {3, 7}, handler))
	, kl1(new King(Piece::WHITE, {3, 0}, handler))
	, qb1(new Queen(Piece::BLACK, {4, 7}, handler))
	, ql1(new Queen(Piece::WHITE, {4, 0}, handler))
{
    mField[0][7] = rb1;
    mField[7][7] = rb2;
    mField[0][0] = rl1;
    mField[7][0] = rl2;

    mField[1][7] = nb1;
    mField[6][7] = nb2;
    mField[1][0] = nl1;
    mField[6][0] = nl2;

    mField[2][7] = bb1;
    mField[5][7] = bb2;
    mField[2][0] = bl1;
    mField[5][0] = bl2;

    mField[4][7] = qb1;
    mField[4][0] = ql1;

    mField[3][7] = kb1;
    mField[3][0] = kl1;

    mField[0][1] = pl1;
    mField[1][1] = pl2;
    mField[2][1] = pl3;
    mField[3][1] = pl4;
    mField[4][1] = pl5;
    mField[5][1] = pl6;
    mField[6][1] = pl7;
    mField[7][1] = pl8;

    mField[0][6] = pb1;
    mField[1][6] = pb2;
    mField[2][6] = pb3;
    mField[3][6] = pb4;
    mField[4][6] = pb5;
    mField[5][6] = pb6;
    mField[6][6] = pb7;
    mField[7][6] = pb8;

    for (int i = 2; i < 6; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            mField[j][i] = nullptr;
        }
    }

    calcAllMoves();
}

Game::~Game()
{
}

Piece* Game::getFieldPos(int row, int col)
{
    return mField[row][col];
}

void Game::move(Piece* start, std::tuple<int, int, Piece::MoveType> move)
{
    if (mCheckEnPassant)
    {
        disableEnPassant();
    }
    else
    {
        mCheckEnPassant = true;
    }

    switch (std::get<2>(move))
    {
        case Piece::NORMAL:
            normal(start->getPos().x, start->getPos().y, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::CASTLE:
            castles(start->getPos().x, start->getPos().y, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::ENPASSANT:
            enPassant(start->getPos().x, start->getPos().y, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::NEWPIECE:
            exchange(start->getPos().x, start->getPos().y, std::get<0>(move), std::get<1>(move));
            break;
        default:
            break;
    }

    gameState();
}

void Game::normal(int xStart, int yStart, int xEnd, int yEnd)
{
    mField[xEnd][yEnd] = getFieldPos(xStart, yStart);
    mField[xEnd][yEnd]->setHasMoved();
    mField[xStart][yStart] = nullptr;
    mSDLHandler.undoPieceRender(xStart, yStart);
    mField[xEnd][yEnd]->setPosition({xEnd, yEnd});
    if (mField[xEnd][yEnd] != nullptr)
    {
        mSDLHandler.undoPieceRender(xEnd, yEnd);
    }
    mField[xEnd][yEnd]->render();

    if (mField[xEnd][yEnd]->getType() == Piece::PAWN)
    {
        if (abs(yEnd - yStart) == 2)
        {
            if (xEnd - 1 >= 0)
            {
                if (mField[xEnd - 1][yEnd] != nullptr)
                {
                    if (mField[xEnd - 1][yEnd]->getType() == Piece::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(mField[xEnd - 1][yEnd]);
                        pwn->setEnPassant(std::pair<bool, int>(true, -1));
                        mCheckEnPassant = false;
                    }
                }
            }

            if (xEnd + 1 <= 7)
            {
                if (mField[xEnd + 1][yEnd] != nullptr)
                {
                    if (mField[xEnd + 1][yEnd]->getType() == Piece::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(mField[xEnd + 1][yEnd]);
                        pwn->setEnPassant(std::pair<bool, int>(true, 1));
                        mCheckEnPassant = false;
                    }
                }
            }
        }
    }
}

void Game::enPassant(int xStart, int yStart, int xEnd, int yEnd)
{
    Pawn* pawn_start = static_cast<Pawn*>(mField[xStart][yStart]);
    mField[xEnd][yEnd - pawn_start->m_dy] = nullptr;
    mField[xEnd][yEnd] = getFieldPos(xStart, yStart);
    mField[xEnd][yEnd]->setHasMoved();
    mField[xStart][yStart] = nullptr;
    mSDLHandler.undoPieceRender(xStart, yStart);
    mSDLHandler.undoPieceRender(xEnd, yEnd - pawn_start->m_dy);
    mField[xEnd][yEnd]->setPosition({xEnd, yEnd});
    mField[xEnd][yEnd]->render();
}

void Game::exchange(int xStart, int yStart, int xEnd, int yEnd)
{
    SDL_Texture* text_rook = mSDLHandler.loadImage(Resources::PieceSprites::kWhiteRook);
    SDL_Texture* text_knight = mSDLHandler.loadImage(Resources::PieceSprites::kWhiteKnight);
    SDL_Texture* text_bishop = mSDLHandler.loadImage(Resources::PieceSprites::kWhiteBishop);
    SDL_Texture* text_queen = mSDLHandler.loadImage(Resources::PieceSprites::kWhiteQueen);
    int y_draw = 0;
    Piece::Team team = Piece::WHITE;

    if (mField[xStart][yStart]->getTeam() == Piece::BLACK)
    {
        text_rook = mSDLHandler.loadImage(Resources::PieceSprites::kBlackRook);
        text_knight = mSDLHandler.loadImage(Resources::PieceSprites::kBlackKnight);
        text_bishop = mSDLHandler.loadImage(Resources::PieceSprites::kBlackBishop);
        text_queen = mSDLHandler.loadImage(Resources::PieceSprites::kBlackQueen);
        y_draw = 3 * mSDLHandler.SCREEN_HEIGHT / 4;
        team = Piece::BLACK;
    }

    SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 155, 103, 60, 255);
    SDL_Rect rectangle = {0,
                          y_draw,
                          mSDLHandler.SCREEN_WIDTH / 4,
                          mSDLHandler.SCREEN_HEIGHT / 4 };
    SDL_RenderFillRect(mSDLHandler.mRenderer, &rectangle);
    SDL_Rect src = { 0, 0, 150, 150 };
    mSDLHandler.drawRectangle(src, rectangle, text_rook);

    SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 255, 255, 255, 255);
    rectangle.x = mSDLHandler.SCREEN_WIDTH / 4;
    SDL_RenderFillRect(mSDLHandler.mRenderer, &rectangle);
    mSDLHandler.drawRectangle(src, rectangle, text_knight);

    SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 155, 103, 60, 255);
    rectangle.x = 2 * mSDLHandler.SCREEN_WIDTH / 4;
    SDL_RenderFillRect(mSDLHandler.mRenderer, &rectangle);
    mSDLHandler.drawRectangle(src, rectangle, text_bishop);

    SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 255, 255, 255, 255);
    rectangle.x = 3 * mSDLHandler.SCREEN_WIDTH / 4;
    SDL_RenderFillRect(mSDLHandler.mRenderer, &rectangle);
    mSDLHandler.drawRectangle(src, rectangle, text_queen);

    bool quit = false;
    int x = -1;
    int y = -1;

    Piece* clickedOn = nullptr;

    //TODO std::cout << m_handler;
    
    while (quit == false)
    {
        while (SDL_PollEvent(&mSDLHandler.mEvent))
        {
            if (mSDLHandler.mEvent.type == SDL_QUIT)
            {
                quit = true;
            }

            if (mSDLHandler.mEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                x = mSDLHandler.mEvent.button.x / 160;
                y = mSDLHandler.mEvent.button.y / 160;
                
                if (y >= y_draw / 160 && y < y_draw / 160 + 1)
                {
                    if (x < mSDLHandler.SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Rook(team, {xEnd, yEnd}, mSDLHandler);
                    }
                    else if (x < 2 * mSDLHandler.SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Knight(team, {xEnd, yEnd}, mSDLHandler);
                    }
                    else if (x < 3 * mSDLHandler.SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Bishop(team, {xEnd, yEnd}, mSDLHandler);
                    }
                    else if (x <= 4 * mSDLHandler.SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Queen(team, {xEnd, yEnd}, mSDLHandler);
                    }
                    std::cout << x << " " << mSDLHandler.SCREEN_WIDTH / 640 << std::endl;
                }
            }
            
            if (mSDLHandler.mEvent.type == SDL_MOUSEBUTTONUP && clickedOn != nullptr)
            {
                quit = true;
            }
        }
    }

    mField[xEnd][yEnd] = clickedOn;
    mField[xStart][yStart] = nullptr;
    mSDLHandler.undoPieceRender(xStart, yStart);
    mSDLHandler.renderBackground();
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mField[i][j] != nullptr)
            {
                mField[i][j]->render();
            }
        }
    }
    
    SDL_DestroyTexture(text_rook);
    SDL_DestroyTexture(text_bishop);
    SDL_DestroyTexture(text_knight);
    SDL_DestroyTexture(text_queen);
}


void Game::castles(int xStart, int yStart, int xEnd, int yEnd)
{
    if (xEnd == 0)
    {
        mField[2][yEnd] = mField[4][yEnd];
        mField[3][yEnd] = mField[0][yEnd];
        mField[2][yEnd]->setHasMoved();
        mField[3][yEnd]->setHasMoved();
        mField[2][yEnd]->setPosition({2, yEnd});
        mField[3][yEnd]->setPosition({3, yEnd});
        mField[4][yEnd] = nullptr;
        mField[0][yEnd] = nullptr;
        mSDLHandler.undoPieceRender(4, yEnd);
        mSDLHandler.undoPieceRender(0, yEnd);
        mField[2][yEnd]->render();
        mField[3][yEnd]->render();
    }
    else
    {
        mField[6][yEnd] = mField[4][yEnd];
        mField[5][yEnd] = mField[7][yEnd];
        mField[6][yEnd]->setHasMoved();
        mField[5][yEnd]->setHasMoved();
        mField[6][yEnd]->setPosition({6, yEnd});
        mField[5][yEnd]->setPosition({5, yEnd});
        mField[4][yEnd] = nullptr;
        mField[7][yEnd] = nullptr;
        mSDLHandler.undoPieceRender(4, yEnd);
        mSDLHandler.undoPieceRender(7, yEnd);
        mField[6][yEnd]->render();
        mField[5][yEnd]->render();
    }
}

void Game::gameState()
{
    bool lost = true;
    King* pivot = kb1;

    if (mTurn == Piece::BLACK)
    {
        pivot = kl1;
    }

    pivot->setCheck(mField, kl1->getPos().x, kl1->getPos().y);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mField[i][j] != nullptr)
            {
                if (mField[i][j]->getTeam() != mTurn)
                {
                    mField[i][j]->calcPossibleMoves(mField, true);
                    if (!mField[i][j]->getPossibleMoves().empty())
                    {
                        lost = false;
                    }
                }
            }
        }
    }

    if (pivot->getCheck() && lost)
    {
        if (mTurn == Piece::BLACK)
        {
            std::cout << "Black wins!";
        }
        else
        {
            std::cout << "White wins!";
        }
    }
    else if (lost)
    {
        if (mTurn == Piece::BLACK)
        {
            std::cout << "Remis!";
        }
        else
        {
            std::cout << "Remis!";
        }
    }
    if (mTurn == Piece::BLACK)
    {
        mTurn = Piece::WHITE;
    }
    else
    {
        mTurn = Piece::BLACK;
    }

}


void Game::disableEnPassant()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mField[i][j] != nullptr)
            {
                if (mField[i][j]->getType() == Piece::PAWN)
                {
                    Pawn* pwn = static_cast<Pawn*>(mField[i][j]);
                    pwn->setEnPassant(std::pair<bool, int>(false, 0));
                }
            }
        }
    }
}


void Game::renderPossibleMoves(Piece* piece)
{
    piece->calcPossibleMoves(mField, true);
    std::vector<Piece::SPieceMovement> possible = piece->getPossibleMoves();
    SDL_Rect rectangle;
    for (const auto& value : possible) {
        if ((value.x % 2 == 0 && value.y % 2 == 1) || (value.x % 2 == 1 && value.y % 2 == 0))
        {
            SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 0, 134, 139, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 164, 211, 238, 255);
        }
        rectangle = { value.x * mSDLHandler.SCREEN_WIDTH / 8,
                      value.y * mSDLHandler.SCREEN_HEIGHT / 8,
                      mSDLHandler.SCREEN_WIDTH / 8,
                      mSDLHandler.SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(mSDLHandler.mRenderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (mField[i][j] != nullptr)
                {
                    mField[i][j]->render();
                }
            }
        }
    } 
}

void Game::undoRenderPossibleMoves(Piece* piece)
{
    std::vector<Piece::SPieceMovement> possible = piece->getPossibleMoves();
    for (const auto& value : possible) {
        if ((value.x % 2 == 0 && value.y % 2 == 1) || (value.x % 2 == 1 && value.y % 2 == 0))
        {
            SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 155, 103, 60, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(mSDLHandler.mRenderer, 255, 255, 255, 255);
        }
        SDL_Rect rectangle = { value.x * mSDLHandler.SCREEN_WIDTH / 8,
                                  value.y * mSDLHandler.SCREEN_HEIGHT / 8,
                                  mSDLHandler.SCREEN_WIDTH / 8,
                                  mSDLHandler.SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(mSDLHandler.mRenderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (mField[i][j] != nullptr)
                {
                    mField[i][j]->render();
                }
            }
        }
    }
}

void Game::calcAllMoves()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mField[i][j] != nullptr)
            {
                mField[i][j]->calcPossibleMoves(mField, true);
            }
        }
    }
}

bool Game::isValidMove(int x, int y, Piece* piece)
{
    std::vector<Piece::SPieceMovement> list = piece->getPossibleMoves();
    for (const auto& value : list) {
        if (value.x == x && value.y == y)
        {
            return true;
        }
    }
    return false;
}
