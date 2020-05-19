#ifndef HOLLOWROUNDEDGRAPHICSPOLYGONITEM_H
#define HOLLOWROUNDEDGRAPHICSPOLYGONITEM_H

#include "enums.h"
#include <QGraphicsPolygonItem>
#include <QPainter>

class HollowRoundedGraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
    HollowRoundedGraphicsPolygonItem(QGraphicsPolygonItem *parent = nullptr) : QGraphicsPolygonItem(parent) {}
    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    enum { Type = HOLLOWROUNDEDGRAPHICSPOLYGONITEM };

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
    void setRoundness(int percentage);
    QStringList getApertureMacro(int decimals);

protected:

    void calculateNgonPoints(void);
    void getMaximumCornerLength(void);
    QPointF getCircleCenter(QPointF firstP, QPointF secondP, QPointF thirdP, int percentage);
    QLineF getCircleTangentLine(QPointF firstCircleCenter, QPointF secondCircleCenter, double circleRadius);
    void buildCircleList(void);
    void buildSplicesList(void);
    void buildCenterPolygon(void);

private:
    double _maxCornerLength = 0.0;
    QRectF _rect = QRectF(0,0,0,0);
    QRectF _hole = QRectF(0,0,0,0);
    QRectF _corner = QRectF(0,0,0,0);
    bool _isHollow = false;
    bool _overrideAutoHollow = false;
    int _vertices = 0;
    float _rotation = 0;
    QPointF _holeCenter = QPointF(0,0);
    QBrush _drawingBrush = QBrush(Qt::SolidPattern);
    QPen _drawingPen = QPen(Qt::SolidLine);
    QVector<QPointF> _ngonPoints;
    QVector<QPointF> _ngonCornerCirclePoints;
    QVector<QPolygonF> _ngonVertexSplices;
    QPolygonF _ngonCenterPolygon;
    double _cornerCircleRadius = 0;
    int _roundness = 0;
};

#endif // HOLLOWROUNDEDGRAPHICSPOLYGONITEM_H
