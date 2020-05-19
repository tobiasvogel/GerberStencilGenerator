#ifndef HOLLOWGRAPHICSOBROUNDITEM_H
#define HOLLOWGRAPHICSOBROUNDITEM_H

#include "enums.h"
#include <QGraphicsEllipseItem>
#include <QPainter>

class HollowGraphicsObroundItem : public QGraphicsEllipseItem
{
public:
    HollowGraphicsObroundItem(QGraphicsEllipseItem *parent = nullptr) : QGraphicsEllipseItem(parent) {}
    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    enum { Type = HOLLOWGRAPHICSOBROUNDITEM };

    int type() const {
        return Type;
    }

    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(QRectF rect);
    void setSize(qreal diameter);
    void setWidth(qreal width);
    void setHeight(qreal height);
    void setHollow(bool);
    bool isHollow(void);
    bool hollow(void);
    void setHoleCenterPos(qreal x, qreal y);
    void setHoleCenterPos(QPointF point);
    void setHoleSize(qreal diameter);
    void setHoleWidth(qreal width);
    void setHoleHeight(qreal height);
    void setHoleRect(QRectF holebb);
    QRectF holeSize(void);
    qreal holeHeight(void);
    qreal holeWidth(void);
    QPointF getHoleCenter(void);
    void setBrush(QBrush brush);
    void setBrush(QColor color);
    void setPen(QColor color);
    void setPen(QPen pen);
    QStringList getApertureMacro();

protected:
    double getShorterSide(void);
    double getLongerSide(void);
    void determineSideRatio(void);

private:
    QRectF _rect = QRectF(0,0,0,0);
    QRectF _hole = QRectF(0,0,0,0);
    bool _isHollow = false;
    bool _overrideAutoHollow = false;
    QPointF _holeCenter = QPointF(0,0);
    QBrush _drawingBrush = QBrush(Qt::SolidPattern);
    QPen _drawingPen = QPen(Qt::SolidLine);
    QRectF _circleA = QRectF(0,0,0,0);
    QRectF _circleB = QRectF(0,0,0,0);
    QRectF _midSection = QRectF(0,0,0,0);

    sides_type _longerSide = UNDETERMINED;
    sides_type _shorterSide = UNDETERMINED;

    void calculateConsistingShapes(void);

};

#endif // HOLLOWGRAPHICSOBROUNDITEM_H
