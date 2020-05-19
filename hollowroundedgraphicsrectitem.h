#ifndef HOLLOWROUNDEDGRAPHICSRECTITEM_H
#define HOLLOWROUNDEDGRAPHICSRECTITEM_H

#include "enums.h"
#include <QGraphicsRectItem>
#include <QPainter>

class HollowRoundedGraphicsRectItem : public QGraphicsRectItem
{
public:
    HollowRoundedGraphicsRectItem(QGraphicsRectItem *parent = nullptr) : QGraphicsRectItem(parent) {}
    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    enum { Type = HOLLOWROUNDEDGRAPHICSRECTITEM };

    int type() const {
        return Type;
    }

    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(QRectF rect);
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
    void setRoundness(int percentage);
    QStringList getApertureMacro(int decimals);

private:
    void getMaximumCornerRadius();
    sides_type getShorterSide();
    double _maxCornerRadius = 0.0;
    QRectF _rect = QRectF(0,0,0,0);
    QRectF _hole = QRectF(0,0,0,0);
    QRectF _corner = QRectF(0,0,0,0);
    bool _isHollow = false;
    bool _overrideAutoHollow = false;
    QPointF _holeCenter = QPointF(0,0);
    QBrush _drawingBrush = QBrush(Qt::SolidPattern);
    QPen _drawingPen = QPen(Qt::SolidLine);
    int _roundness = 0;
};

#endif // HOLLOWROUNDEDGRAPHICSRECTITEM_H
