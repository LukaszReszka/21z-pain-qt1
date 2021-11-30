#include "gameboard.h"
#include <QThread>

GameBoard::GameBoard(QWidget *parent) : QGraphicsView(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gameScene = new QGraphicsScene(0, 0, BOARD_DIMENSION, BOARD_DIMENSION, this);
    int x_field, y_field = 0;
    for(int row = 0; row < FIELDS_IN_ROW; ++row)
    {
        x_field = 0;
        for(int column = 0; column < FIELDS_IN_COL; ++column)
        {
            if((column+ row)%2)
                board[row][column] = new BoardField(lightFieldTexture);
            else
                board[row][column] = new BoardField(darkFieldTexture);

            board[row][column] -> setPos(gameScene->sceneRect().left() + x_field,
                                           gameScene->sceneRect().top() + y_field);
            board[row][column]->setPawnsTextures(whitePawnTexture, blackPawnTexture);
            gameScene -> addItem(board[row][column]);
            x_field += FIELD_DIMENSION;
            QObject::connect(board[row][column], &BoardField::putPawn,
                             this, &GameBoard::emitToGameEngine);
        }
        y_field += FIELD_DIMENSION;
    }

    resetBoard();
    setScene(gameScene);
}

void GameBoard::setPawnStatusOnField(int col, int row, PawnStatus status) {
    board[row][col]->setPawnOnField(status);
}

void GameBoard::resetBoard() {
    for(int r = 0; r < FIELDS_IN_ROW; ++r)
        for(int c = 0; c < FIELDS_IN_COL; ++c)
            board[r][c]->setPawnOnField(PawnStatus::NONE);

    board[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2-1]->setPawnOnField(PawnStatus::WHITE);
    board[FIELDS_IN_ROW/2][FIELDS_IN_COL/2]->setPawnOnField(PawnStatus::WHITE);
    board[FIELDS_IN_ROW/2-1][FIELDS_IN_COL/2]->setPawnOnField(PawnStatus::BLACK);
    board[FIELDS_IN_ROW/2][FIELDS_IN_COL/2-1]->setPawnOnField(PawnStatus::BLACK);
}

GameBoard::~GameBoard()
{
    delete gameScene;
}

void GameBoard::repaintBoard() {
    for(int r = 0; r < FIELDS_IN_ROW; ++r)
        for(int c = 0; c < FIELDS_IN_COL; ++c)
            board[r][c]->update();
}
