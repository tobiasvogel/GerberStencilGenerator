#include "hollowroundedgraphicspolygonitem.h"
#include <QtMath>
#include <QDebug>
#include "floatcompare.h"


QRectF HollowRoundedGraphicsPolygonItem::boundingRect() const
{
    return _rect;
}

void HollowRoundedGraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
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
    painter->drawPolygon(_ngonCenterPolygon);
    for (int i = 0; i < _vertices; i++) {
        painter->drawEllipse(_ngonCornerCirclePoints.at(i),_cornerCircleRadius,_cornerCircleRadius);
        painter->drawPolygon(_ngonVertexSplices.at(i));
    }
//    painter->drawRect(_rect);
}

void HollowRoundedGraphicsPolygonItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    _rect.setX(x);
    _rect.setY(y);
    _rect.setWidth(w);
    _rect.setHeight(h);
}

void HollowRoundedGraphicsPolygonItem::setRect(QRectF rect)
{
    _rect.setX(rect.x());
    _rect.setY(rect.y());
    _rect.setWidth(rect.width());
    _rect.setHeight(rect.height());
}

void HollowRoundedGraphicsPolygonItem::setRadius(qreal radius)
{
    _rect.setX(-radius);
    _rect.setY(-radius);
    _rect.setWidth(radius*2);
    _rect.setHeight(radius*2);
}

void HollowRoundedGraphicsPolygonItem::setCircumference(qreal diameter)
{
    this->setRadius(diameter/2);
}

void HollowRoundedGraphicsPolygonItem::setHollow(bool hollow)
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

bool HollowRoundedGraphicsPolygonItem::isHollow()
{
    return _isHollow;
}

bool HollowRoundedGraphicsPolygonItem::hollow()
{
    return _isHollow;
}

void HollowRoundedGraphicsPolygonItem::setHoleCenterPos(qreal x, qreal y)
{
    _holeCenter.setX(x);
    _holeCenter.setY(-y);
}

void HollowRoundedGraphicsPolygonItem::setHoleCenterPos(QPointF point)
{
    _holeCenter.setX(point.x());
    _holeCenter.setY(-point.y());
}

void HollowRoundedGraphicsPolygonItem::setHoleSize(qreal diameter)
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

void HollowRoundedGraphicsPolygonItem::setHoleWidth(qreal width)
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

void HollowRoundedGraphicsPolygonItem::setHoleHeight(qreal height)
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

void HollowRoundedGraphicsPolygonItem::setHoleRect(QRectF holebb)
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

QRectF HollowRoundedGraphicsPolygonItem::holeSize()
{
    return _hole;
}

qreal HollowRoundedGraphicsPolygonItem::holeHeight()
{
    return _hole.height();
}

qreal HollowRoundedGraphicsPolygonItem::holeWidth()
{
    return _hole.width();
}

QPointF HollowRoundedGraphicsPolygonItem::getHoleCenter()
{
    return _holeCenter;
}

void HollowRoundedGraphicsPolygonItem::setBrush(QBrush brush)
{
    _drawingBrush = brush;
}

void HollowRoundedGraphicsPolygonItem::setBrush(QColor color)
{
    _drawingBrush.setColor(color);
}

void HollowRoundedGraphicsPolygonItem::setPen(QColor color)
{
    _drawingPen.setColor(color);
}

void HollowRoundedGraphicsPolygonItem::setPen(QPen pen)
{
    _drawingPen = pen;
}


void HollowRoundedGraphicsPolygonItem::setVertices(int v)
{
    if (v > 2) {
        _vertices = v;
    } else {
        _vertices = 0;
    }
}

void HollowRoundedGraphicsPolygonItem::setRotation(float r)
{
    if (fl_equal(r,0.0)) {
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

void HollowRoundedGraphicsPolygonItem::setRoundness(int percentage)
{
        _roundness = percentage;
}

void HollowRoundedGraphicsPolygonItem::calculateNgonPoints()
{
    _ngonPoints.clear();
    if (_rect.width() <= 0 || _rect.height() <= 0) { return; }

    double radius = (_rect.width()/2);
    QPointF center = QPointF(_rect.center());
    double angle = 0;
    int vertices = _vertices;
    double startAngle = 0;
    double centerAngle = ((2*M_PI) / vertices);

    if (fl_equal(_rotation,0.0)) {
    if (vertices % 2 == 1) {
        startAngle = M_PI_2;
    } else {
        startAngle = M_PI_2 - centerAngle/2;
    }
    } else {
        startAngle = static_cast<double>(qDegreesToRadians(_rotation));
    }

    for (int i = 0; i < vertices; i++) {
        angle = startAngle + (i*centerAngle);
        QPointF tempPoint;
        tempPoint.setX(center.x()+radius*qCos(angle));
        tempPoint.setY(center.y()-radius*qSin(angle));
        _ngonPoints.append(tempPoint);
    }

    this->getMaximumCornerLength();
    this->buildCircleList();
    this->buildSplicesList();
    this->buildCenterPolygon();

}

void HollowRoundedGraphicsPolygonItem::getMaximumCornerLength()
{
    QPointF pointA = _ngonPoints.at(0);
    QPointF pointB = _ngonPoints.at(1);
    double distance = qSqrt(qPow((pointB.x()-pointA.x()),2)+qPow((pointB.y()-pointA.y()),2));
    _maxCornerLength = distance/2;
}

QPointF HollowRoundedGraphicsPolygonItem::getCircleCenter(QPointF firstP, QPointF secondP, QPointF thirdP, int percentage)
{
    std::ignore = thirdP;

    QPointF firstCenterP = QPointF(((firstP.x()+secondP.x())/2),((firstP.y()+secondP.y())/2));
    double cornerDistance = _maxCornerLength/100*percentage;
    double distanceRatio = cornerDistance/_maxCornerLength;
    QPointF firstCircleP = QPointF(((1-distanceRatio)*secondP.x()+distanceRatio*firstCenterP.x()),((1-distanceRatio)*secondP.y()+distanceRatio*firstCenterP.y()));
    double maxCornerRadius = qSqrt(qPow(firstCenterP.x(),2)+qPow(firstCenterP.y(),2));
    double circleCenterDistance = maxCornerRadius/100*percentage;
    double centerDistanceRatio = circleCenterDistance/maxCornerRadius;
    QPointF centerCircleP = QPointF(((1-centerDistanceRatio)*secondP.x()),((1-centerDistanceRatio)*secondP.y()));

    if (_cornerCircleRadius == 0.0) {
        _cornerCircleRadius = qSqrt(qPow((centerCircleP.x()-firstCircleP.x()),2)+qPow((centerCircleP.y()-firstCircleP.y()),2));
    }

    return centerCircleP;
}

QLineF HollowRoundedGraphicsPolygonItem::getCircleTangentLine(QPointF firstCircleCenter, QPointF secondCircleCenter, double circleRadius)
{
    double dX = secondCircleCenter.x() - firstCircleCenter.x();
    double dY = secondCircleCenter.y() - firstCircleCenter.y();
    double angle = qAtan2(dX,dY);
    double distX = circleRadius * qCos(angle);
    double distY = -circleRadius * qSin(angle);
    QLineF tempLine(firstCircleCenter, secondCircleCenter);
    tempLine.translate(-distX, -distY);
    return tempLine;
}

void HollowRoundedGraphicsPolygonItem::buildCircleList()
{
    _cornerCircleRadius = 0.0;
    _ngonCornerCirclePoints.clear();
    for (int i = 0; i < _vertices; i++) {
        int j = i - 1;
        int k = i;
        int l = i + 1;
        if (j < 0) { j = _vertices-1; }
        if (l == _vertices) { l = 0; }
        QPointF tempCircleCenter = getCircleCenter(_ngonPoints.at(j), _ngonPoints.at(k), _ngonPoints.at(l), _roundness);
        _ngonCornerCirclePoints.append(tempCircleCenter);
    }
}

void HollowRoundedGraphicsPolygonItem::buildSplicesList()
{
    _ngonVertexSplices.clear();
    for (int i = 0; i < _vertices; i++) {
        int j = i-1;
        int k = i;
        if (j < 0) { j = _vertices-1; }
        QPolygonF tempSplice;
        QLineF tempTangent = getCircleTangentLine(_ngonCornerCirclePoints.at(j), _ngonCornerCirclePoints.at(k), _cornerCircleRadius);
        tempSplice.append(QPointF(_rect.center()));
        tempSplice.append(tempTangent.p1());
        tempSplice.append(tempTangent.p2());
        _ngonVertexSplices.append(tempSplice);
    }
}

void HollowRoundedGraphicsPolygonItem::buildCenterPolygon()
{
    _ngonCenterPolygon.clear();
    Q_FOREACH (QPointF p, _ngonCornerCirclePoints) {
        _ngonCenterPolygon.append(p);
    }
}
