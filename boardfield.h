#ifndef BOARDFIELD_H
#define BOARDFIELD_H

#include <QGraphicsItem>
#include <QBrush>
#include "pawnstatus.h"

const int FIELD_DIMENSION = 64;

class BoardField : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit BoardField(QBrush &texture, QGraphicsItem *parent = nullptr);
    virtual ~BoardField() {};
    QRectF	boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
     *option, QWidget *widget) override;
    void setPawnsTextures(QBrush &whitePawn, QBrush &BlackPawn);
    void setPawnOnField(PawnStatus kindOfPawn);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void putPawn(int col, int row);

private:
    const int PAWN_DIAMETER = 50;
    const int PAWN_POSITION = (FIELD_DIMENSION-PAWN_DIAMETER)/2;

    QBrush fieldTexture, blackPawnTextr, whitePawnTextr;
    PawnStatus pawnOnField = PawnStatus::NONE;
};

#endif // BOARDFIELD_H
