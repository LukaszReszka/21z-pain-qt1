#include "boardfield.h"
#include <QPainter>

BoardField::BoardField(QBrush &texture, QGraphicsItem *parent)
    :fieldTexture(texture) , QGraphicsItem(parent) {}

QRectF	BoardField::boundingRect() const {
    return QRectF(0, 0, FIELD_DIMENSION, FIELD_DIMENSION);
}

void BoardField::paint(QPainter *painter, const QStyleOptionGraphicsItem
           *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->fillRect(boundingRect(), fieldTexture);

    if(pawnOnField == PawnStatus::WHITE) {
        painter->setBrush(whitePawnTextr);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(PAWN_POSITION, PAWN_POSITION, PAWN_DIAMETER, PAWN_DIAMETER);
    }
    else if (pawnOnField == PawnStatus::BLACK)
    {
        painter->setBrush(blackPawnTextr);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(PAWN_POSITION,PAWN_POSITION, PAWN_DIAMETER, PAWN_DIAMETER);
    }
}

void BoardField::setPawnsTextures(QBrush &whitePawn, QBrush &blackPawn)
{
    whitePawnTextr = whitePawn;
    blackPawnTextr = blackPawn;
}

void BoardField::setPawnOnField(PawnStatus kindOfPawn)
{
    pawnOnField = kindOfPawn;
}

void BoardField::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    int x = this->pos().rx()/FIELD_DIMENSION;
    int y = this->pos().ry()/FIELD_DIMENSION;

    emit putPawn(x, y);
}
