#include "gameengine.h"

GameEngine::GameEngine(GameBoard * b) : board(b) {
    resetGame();
}

void GameEngine::resetGame() {
    for(int r = 0; r < FIELDS_IN_ROW; ++r)
        for(int c = 0; c < FIELDS_IN_COL; ++c) {
            whitesMovesToCheckBoard[r][c] = NOT_TO_CHECK;
            blacksMovesToCheckBoard[r][c] = NOT_TO_CHECK;
            avaiableMoves[r][c] = "";
        }

    whitesMovesToCheckBoard[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2-1] = WHITE_PAWN;
    blacksMovesToCheckBoard[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2-1] = WHITE_PAWN;

    whitesMovesToCheckBoard[FIELDS_IN_ROW/2][FIELDS_IN_COL/2] = WHITE_PAWN;
    blacksMovesToCheckBoard[FIELDS_IN_ROW/2][FIELDS_IN_COL/2] = WHITE_PAWN;

    whitesMovesToCheckBoard[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2]= BLACK_PAWN;
    blacksMovesToCheckBoard[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2]= BLACK_PAWN;

    whitesMovesToCheckBoard[FIELDS_IN_ROW/2][FIELDS_IN_COL/2-1]= BLACK_PAWN;
    blacksMovesToCheckBoard[FIELDS_IN_ROW/2][FIELDS_IN_COL/2-1]= BLACK_PAWN;

    markNeighbours(FIELDS_IN_ROW/2-1, FIELDS_IN_COL/2-1, true, blacksMovesToCheckBoard);
    markNeighbours(FIELDS_IN_ROW/2, FIELDS_IN_COL/2, true, blacksMovesToCheckBoard);

    markNeighbours(FIELDS_IN_ROW/2-1, FIELDS_IN_COL/2, true, whitesMovesToCheckBoard);
    markNeighbours(FIELDS_IN_ROW/2, FIELDS_IN_COL/2-1, true, whitesMovesToCheckBoard);

    isBlacksTurn = true;
    areMovesToMake = true;
    hasGameEnded = false;

    avaiableMoves[FIELDS_IN_ROW/2-2][FIELDS_IN_COL/2-1] += S;
    avaiableMoves[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2-2] += E;
    avaiableMoves[FIELDS_IN_ROW/2+1][FIELDS_IN_COL/2] += N;
    avaiableMoves[FIELDS_IN_ROW/2][FIELDS_IN_COL/2+1] += W;

    emit setStatusBarText("Gra czarnych pionków");

    board->resetBoard();
    board->repaintBoard();
}


void GameEngine::markNeighbours(int row, int col, bool isWorthChecking,
             int tab[FIELDS_IN_ROW][FIELDS_IN_COL]) {

    int min_x = col - 1 >= 0 ? col - 1 : 0;
    int min_y = row - 1 >= 0 ? row - 1 : 0;

    int max_x = col + 1 < FIELDS_IN_COL ? col + 1 : FIELDS_IN_COL - 1;
    int max_y = row + 1 < FIELDS_IN_ROW ? row + 1 : FIELDS_IN_ROW - 1;

    for (int y = min_y; y <= max_y; ++y)
        for (int x = min_x; x <= max_x; ++x)
            if (tab[y][x] >= NOT_TO_CHECK)
            {
                if(isWorthChecking)
                    tab [y][x] += 1;
                else
                    tab [y][x] -= 1;
            }
}

void GameEngine::clearAvaiableMovesTab() {
    for(int r = 0; r < FIELDS_IN_ROW; ++r)
        for(int c = 0; c < FIELDS_IN_COL; ++c)
            avaiableMoves[r][c] = "";
}

void GameEngine::checkPosition(int row, int col) {
    for(int delta_y = -1; delta_y <= 1; ++delta_y) {
        for(int delta_x = -1; delta_x <= 1; ++delta_x) {
            bool isAvaiableDirection = false;
            if(delta_x == 0 && delta_y == 0)
                continue;

            int y = row + delta_y;
            int x = col + delta_x;
            if(x < 0 || y < 0 || x >= FIELDS_IN_COL || y >= FIELDS_IN_ROW)
                continue;

            if(isBlacksTurn) {
                if(blacksMovesToCheckBoard[y][x] != WHITE_PAWN)
                    continue;

                while(!isAvaiableDirection && x >= 0 && x < FIELDS_IN_COL
                      && y >= 0 && y < FIELDS_IN_ROW) {
                    if(blacksMovesToCheckBoard[y][x] == BLACK_PAWN)
                        isAvaiableDirection = true;
                    else {
                    y += delta_y;
                    x += delta_x;
                    }
                }

            }
            else {
                if(whitesMovesToCheckBoard[y][x] != BLACK_PAWN)
                    continue;
                while(!isAvaiableDirection && x >= 0 && x < FIELDS_IN_COL
                      && y >= 0 && y < FIELDS_IN_ROW) {
                    if(whitesMovesToCheckBoard[y][x] == WHITE_PAWN)
                        isAvaiableDirection = true;
                    else {
                    y += delta_y;
                    x += delta_x;
                    }
                }
            }

            if(isAvaiableDirection)     //saving avaiable direction
            {
                if(delta_y == -1) {
                    if (delta_x == -1)
                        avaiableMoves[row][col] += NW;
                    else if(delta_x == 1)
                        avaiableMoves[row][col] += NE;
                    else
                        avaiableMoves[row][col] += N;

                }
                else if(delta_y == 1) {
                    if (delta_x == -1)
                        avaiableMoves[row][col] += SW;
                    else if(delta_x == 1)
                        avaiableMoves[row][col] += SE;
                    else
                        avaiableMoves[row][col] += S;
                }
                else {
                    if(delta_x == -1)
                        avaiableMoves[row][col] += W;
                    else
                       avaiableMoves[row][col] += E;
                }
            }
        }
    }
}

void GameEngine::markAllAvaiablePositions() {
    areMovesToMake = false;
    if(isBlacksTurn) {
        for(int r = 0; r < FIELDS_IN_ROW; ++r)
            for(int c = 0; c < FIELDS_IN_COL; ++c)
                if(blacksMovesToCheckBoard[r][c] > NOT_TO_CHECK)
                {
                    checkPosition(r, c);
                    areMovesToMake = true;
                }
    } else {
        for(int r = 0; r < FIELDS_IN_ROW; ++r)
            for(int c = 0; c < FIELDS_IN_COL; ++c)
                if(whitesMovesToCheckBoard[r][c] > NOT_TO_CHECK)
                {
                    checkPosition(r, c);
                    areMovesToMake = true;
                }
    }
}

void GameEngine::swapPawnsInLine(int start_x, int start_y, char direction)
{
    int delta_x = 0, delta_y = 0, pawn_color, x = start_x, y = start_y;

    if(direction == N) {
        delta_y = -1;
    }
    else if(direction == S) {
        delta_y = 1;
    }
    else if(direction == E) {
        delta_x = 1;
    }
    else if(direction == W) {
        delta_x = -1;
    }
    else if(direction == NE) {
        delta_x = 1;
        delta_y = -1;
    }
    else if(direction == NW) {
        delta_x = -1;
        delta_y = -1;
    }
    else if(direction == SE) {
        delta_x = 1;
        delta_y = 1;
    }
    else if(direction == SW) {
        delta_x = -1;
        delta_y = 1;
    }

    if(isBlacksTurn)
        pawn_color = BLACK_PAWN;
    else
        pawn_color = WHITE_PAWN;

    x += delta_x;
    y += delta_y;

    do {

        if(pawn_color == BLACK_PAWN) {
            markNeighbours(y, x, true, whitesMovesToCheckBoard);
            markNeighbours(y, x, false, blacksMovesToCheckBoard);
        } else {
            markNeighbours(y, x, false, whitesMovesToCheckBoard);
            markNeighbours(y, x, true, blacksMovesToCheckBoard);
        }

        whitesMovesToCheckBoard[y][x] = pawn_color;
        blacksMovesToCheckBoard[y][x] = pawn_color;
        board->setPawnStatusOnField(x, y, isBlacksTurn? PawnStatus::BLACK : PawnStatus::WHITE);

        x += delta_x;
        y += delta_y;
    } while(whitesMovesToCheckBoard[y][x] != pawn_color);
}

void GameEngine::swapPawns(int field_x, int field_y) {
    std::string swapping_directions = avaiableMoves[field_y][field_x];

    if(isBlacksTurn) {
        markNeighbours(field_y, field_x, true, whitesMovesToCheckBoard);
        markNeighbours(field_y, field_x, false, blacksMovesToCheckBoard);
        whitesMovesToCheckBoard[field_y][field_x] = BLACK_PAWN;
        blacksMovesToCheckBoard[field_y][field_x] = BLACK_PAWN;
        board->setPawnStatusOnField(field_x, field_y, PawnStatus::BLACK);
    } else {
        markNeighbours(field_y, field_x, false, whitesMovesToCheckBoard);
        markNeighbours(field_y, field_x, true, blacksMovesToCheckBoard);
        whitesMovesToCheckBoard[field_y][field_x] = WHITE_PAWN;
        blacksMovesToCheckBoard[field_y][field_x] = WHITE_PAWN;
        board->setPawnStatusOnField(field_x, field_y, PawnStatus::WHITE);
    }


    for (char dir: swapping_directions) {
        swapPawnsInLine(field_x, field_y, dir);
    }
}

void GameEngine::processPlayersMove(int x, int y) {
    if( !hasGameEnded && avaiableMoves[y][x] != "")
    {
        swapPawns(x, y);
        board->repaintBoard();
        clearAvaiableMovesTab();
        isBlacksTurn = !isBlacksTurn;
        markAllAvaiablePositions();

        if(isBlacksTurn) {
            emit setStatusBarText("Gra czarnych pionków");
        }
        else {
            emit setStatusBarText("Gra białych pionków");
        }


        if(!areMovesToMake) {   //cannot make move
            isBlacksTurn = !isBlacksTurn;
            clearAvaiableMovesTab();
            markAllAvaiablePositions();

            if(isBlacksTurn) {
                emit setStatusBarText("Gra czarnych pionków");
            }
            else {
                emit setStatusBarText("Gra białych pionków");
            }

            if(!areMovesToMake) //end of the game
            {
                hasGameEnded = true;
                emit setStatusBarText("Koniec gry");
                return;
            }
        }

    }
}
