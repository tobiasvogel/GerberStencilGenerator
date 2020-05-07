#ifndef HOLLOWGRAPHICSRECTITEM_H
#define HOLLOWGRAPHICSRECTITEM_H

#include <QGraphicsRectItem>
#include <QPainter>

class HollowGraphicsRectItem : public QGraphicsRectItem
{
public:
    HollowGraphicsRectItem(QGraphicsRectItem *parent = nullptr) : QGraphicsRectItem(parent) {}
    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

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
    QStringList getApertureMacro(QRectF boundingBox, QRectF holeSize, QPointF centerPoint, QPointF holeCenter, int roundness, int vertices,  bool isHollow);

private:
    QRectF _rect = QRectF(0,0,0,0);
    QRectF _hole = QRectF(0,0,0,0);
    bool _isHollow = false;
    bool _overrideAutoHollow = false;
    QPointF _holeCenter = QPointF(0,0);
    QBrush _drawingBrush = QBrush(Qt::SolidPattern);
    QPen _drawingPen = QPen(Qt::SolidLine);
};

#endif // HOLLOWGRAPHICSRECTITEM_H
