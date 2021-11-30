#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "boardfield.h"

const int FIELDS_IN_ROW = 8;
const int FIELDS_IN_COL = FIELDS_IN_ROW;
const int BOARD_DIMENSION = 8 * FIELD_DIMENSION;

class GameBoard : public QGraphicsView
{
    Q_OBJECT
public:
    GameBoard(QWidget *parent);
    ~GameBoard();

bool hasHeightForWidth() const override {return true;}
int heightForWidth(int w) const override {return w;}
QSize sizeHint() const override {return QSize(BOARD_DIMENSION, BOARD_DIMENSION);}
void setPawnStatusOnField(int x, int y, PawnStatus status);
void resetBoard();
void repaintBoard();

public slots:
void emitToGameEngine(int col, int row) {
    emit putPawnOnField(col, row);
}

signals:
    void putPawnOnField(int col, int row);

private:
    QGraphicsScene *gameScene;
    BoardField *board [FIELDS_IN_ROW][FIELDS_IN_COL];
    QBrush darkFieldTexture = QBrush(QPixmap(":/dark_field_texture.bmp"));
    QBrush lightFieldTexture = QBrush(QPixmap(":/light_field_texture.bmp"));
    QBrush blackPawnTexture = QBrush(QPixmap(":/black_pawn_texture.bmp"));
    QBrush whitePawnTexture = QBrush(QPixmap(":/white_pawn_texture.bmp"));
};

#endif // GAMEBOARD_H
