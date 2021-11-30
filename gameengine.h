#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include<QObject>
#include "pawnstatus.h"
#include "gameboard.h"

class GameEngine: public QObject
{
    Q_OBJECT
public:
    GameEngine(GameBoard * b);
    void resetGame();

private:
    GameBoard *board;
    int whitesMovesToCheckBoard[FIELDS_IN_ROW][FIELDS_IN_COL];
    int blacksMovesToCheckBoard[FIELDS_IN_ROW][FIELDS_IN_COL];
    std::string avaiableMoves[FIELDS_IN_ROW][FIELDS_IN_COL];
    bool isBlacksTurn;
    bool areMovesToMake;
    bool hasGameEnded;
    enum {
     BLACK_PAWN = -2, WHITE_PAWN = -1, NOT_TO_CHECK = 0,
     N = '0', S = '1', E = '2', W = '3', NE = '4', NW = '5', SE = '6', SW = '7'
    };

    void markNeighbours(int row, int col, bool isWorthChecking,
                                  int tab[FIELDS_IN_ROW][FIELDS_IN_COL]);
    void clearAvaiableMovesTab();
    void markAllAvaiablePositions();
    void checkPosition(int row, int col);
    void swapPawnsInLine(int start_x, int start_y, char direction);
    void swapPawns(int field_x, int field_y);


    public slots:
    void processPlayersMove(int x, int y);

signals:
    void setStatusBarText(const QString message, int timeout = 0);

};

#endif // GAMEENGINE_H
