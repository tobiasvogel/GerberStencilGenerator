#ifndef HOLLOWGRAPHICSPOLYGONITEM_H
#define HOLLOWGRAPHICSPOLYGONITEM_H

#include "enums.h"
#include <QGraphicsPolygonItem>
#include <QPainter>

class HollowGraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
    HollowGraphicsPolygonItem(QGraphicsPolygonItem *parent = nullptr) : QGraphicsPolygonItem(parent) {}
    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    enum { Type = HOLLOWGRAPHICSPOLYGONITEM };

    int type() const {
        return Type;
    }

    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(QRectF rect);
    void setRadius(qreal radius);
    void setCircumference(qreal diameter);
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
    void setVertices(int v);
    void setRotation(float r);
    QStringList getApertureMacro();

protected:

    void calculateNgonPoints(void);

private:
    QRectF _rect = QRectF(0,0,0,0);
    QRectF _hole = QRectF(0,0,0,0);
    bool _isHollow = false;
    bool _overrideAutoHollow = false;
    int _vertices = 0;
    float _rotation = 0;
    QPointF _holeCenter = QPointF(0,0);
    QBrush _drawingBrush = QBrush(Qt::SolidPattern);
    QPen _drawingPen = QPen(Qt::SolidLine);
    QVector<QPointF> _ngonPoints;
};

#endif // HOLLOWGRAPHICSPOLYGONITEM_H
