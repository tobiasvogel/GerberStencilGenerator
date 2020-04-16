#include "hollowgraphicspolygonitem.h"
#include <QtMath>
#include <QDebug>


QRectF HollowGraphicsPolygonItem::boundingRect() const
{
    return _rect;
}

void HollowGraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
{
    std::ignore = option;
    std::ignore = widget;

    this->calculateNgonPoints();

    QPainterPath scene;
    scene.addRect(_rect);
    QPainterPath mask;
    QRectF maskEllipse;
    maskEllipse = _hole;
    maskEllipse.moveCenter(_holeCenter);
    mask.addEllipse(maskEllipse);
    mask = scene.subtracted(mask);

    painter->setClipPath(mask);

    painter->setPen(_drawingPen);
    painter->setBrush(_drawingBrush);
    painter->drawPolygon(_ngonPoints);
}

void HollowGraphicsPolygonItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    _rect.setX(x);
    _rect.setY(y);
    _rect.setWidth(w);
    _rect.setHeight(h);
}

void HollowGraphicsPolygonItem::setRect(QRectF rect)
{
    _rect.setX(rect.x());
    _rect.setY(rect.y());
    _rect.setWidth(rect.width());
    _rect.setHeight(rect.height());
}

void HollowGraphicsPolygonItem::setRadius(qreal radius)
{
    _rect.setX(-radius);
    _rect.setY(-radius);
    _rect.setWidth(radius*2);
    _rect.setHeight(radius*2);
}

void HollowGraphicsPolygonItem::setCircumference(qreal diameter)
{
    this->setRadius(diameter/2);
}

void HollowGraphicsPolygonItem::setHollow(bool hollow)
{
    if (_isHollow == true && hollow == false) {
        _overrideAutoHollow = true;
    }
    _isHollow = hollow;
    if (_hole.height() != 0.0 && _hole.width() != 0.0) {
        _isHollow = true;
        if (hollow == true && _overrideAutoHollow == true) {
            _overrideAutoHollow = false;
        }
    } else {
        _isHollow = false;
    }

}

bool HollowGraphicsPolygonItem::isHollow()
{
    return _isHollow;
}

bool HollowGraphicsPolygonItem::hollow()
{
    return _isHollow;
}

void HollowGraphicsPolygonItem::setHoleCenterPos(qreal x, qreal y)
{
    _holeCenter.setX(x);
    _holeCenter.setY(-y);
}

void HollowGraphicsPolygonItem::setHoleCenterPos(QPointF point)
{
    _holeCenter.setX(point.x());
    _holeCenter.setY(-point.y());
}

void HollowGraphicsPolygonItem::setHoleSize(qreal diameter)
{
    _hole.setX(-(diameter/2));
    _hole.setY(-(diameter/2));
    _hole.setWidth(diameter);
    _hole.setHeight(diameter);
    if (_hole.height() != 0.0 && _hole.width() != 0.0) {
        if (!_overrideAutoHollow) {
            _isHollow = true;
        }
    } else {
        _isHollow = false;
    }
}

void HollowGraphicsPolygonItem::setHoleWidth(qreal width)
{
    _hole.setWidth(width);
    _hole.setX(-(width/2));
    if (_hole.height() != 0.0 && width != 0.0) {
        if (!_overrideAutoHollow) {
            _isHollow = true;
        }
    } else {
        _isHollow = false;
    }
}

void HollowGraphicsPolygonItem::setHoleHeight(qreal height)
{
    _hole.setHeight(height);
    _hole.setY(-(height/2));
    if (_hole.width() != 0.0 && height != 0.0) {
        if (!_overrideAutoHollow) {
            _isHollow = true;
        }
    } else {
        _isHollow = false;
    }
}

void HollowGraphicsPolygonItem::setHoleRect(QRectF holebb)
{
    _hole.setX(holebb.x());
    _hole.setY(holebb.y());
    _hole.setWidth(holebb.width());
    _hole.setHeight(holebb.height());
    if (_hole.height() != 0.0 && _hole.width() != 0.0) {
        if (!_overrideAutoHollow) {
            _isHollow = true;
        }
    } else {
        _isHollow = false;
    }
}

QRectF HollowGraphicsPolygonItem::holeSize()
{
    return _hole;
}

qreal HollowGraphicsPolygonItem::holeHeight()
{
    return _hole.height();
}

qreal HollowGraphicsPolygonItem::holeWidth()
{
    return _hole.width();
}

QPointF HollowGraphicsPolygonItem::getHoleCenter()
{
    return _holeCenter;
}

void HollowGraphicsPolygonItem::setBrush(QBrush brush)
{
    _drawingBrush = brush;
}

void HollowGraphicsPolygonItem::setBrush(QColor color)
{
    _drawingBrush.setColor(color);
}

void HollowGraphicsPolygonItem::setPen(QColor color)
{
    _drawingPen.setColor(color);
}

void HollowGraphicsPolygonItem::setPen(QPen pen)
{
    _drawingPen = pen;
}


void HollowGraphicsPolygonItem::setVertices(int v)
{
    if (v > 2) {
        _vertices = v;
    } else {
        _vertices = 0;
    }
}

void HollowGraphicsPolygonItem::setRotation(float r)
{
    if (r == 0) {
        _rotation = 0;
    } else if (r < 0) {
        float remainder;
        remainder = qAbs(r);
        int i = 99;
        while (remainder > 360 && i > 0) {
            remainder -= 360;
            i--;
        }
        if (i > 0) {
        _rotation = 360 - remainder;
        }
    } else {
        int i = 99;
        if (r > 360) {
            float remainder = r;
            while (remainder > 360 && i > 0) {
                remainder -= 360;
                i--;
            }
            if (i > 0) {
                _rotation = remainder;
            }
        } else {
            _rotation = r;
        }
    }
}

void HollowGraphicsPolygonItem::calculateNgonPoints()
{
    _ngonPoints.clear();
    if (_rect.width() <= 0 || _rect.height() <= 0) { return; }

    double radius = (_rect.width()/2);
    QPointF center = QPointF(_rect.center());
    double angle = 0;
    int vertices = _vertices;
    double startAngle = 0;
    double centerAngle = ((2*M_PI) / vertices);

    if (_rotation == 0) {
    if (vertices % 2 == 1) {
        startAngle = M_PI_2;
    } else {
        startAngle = M_PI_2 - centerAngle/2;
    }
    } else {
        startAngle = qDegreesToRadians(_rotation);
    }

    for (int i = 0; i < vertices; i++) {
        angle = startAngle + (i*centerAngle);
        QPointF tempPoint;
        tempPoint.setX(center.x()+radius*qCos(angle));
        tempPoint.setY(center.y()-radius*qSin(angle));
        _ngonPoints.append(tempPoint);
    }

}
