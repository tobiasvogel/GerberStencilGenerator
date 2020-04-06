#include "hollowroundedgraphicsrectitem.h"


QRectF HollowRoundedGraphicsRectItem::boundingRect() const
{
    return _rect;
}

void HollowRoundedGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
{

    getMaximumCornerRadius();

    double radius = _maxCornerRadius/100*_roundness;
    double width = _rect.width();
    double height = _rect.height();

    QRectF cornerRect(-radius, -radius, radius*2, radius*2);

    QPainterPath scene;
    scene.addRect(_rect);

    QRectF topLeftCorner = cornerRect;
    QRectF topRightCorner = cornerRect;
    QRectF bottomRightCorner = cornerRect;
    QRectF bottomLeftCorner = cornerRect;

    topLeftCorner.moveCenter(QPointF((-(width/2)+radius),(-(height/2)+radius)));
    topRightCorner.moveCenter(QPointF(((width/2)-radius),(-(height/2)+radius)));
    bottomRightCorner.moveCenter(QPointF(((width/2)-radius),((height/2)-radius)));
    bottomLeftCorner.moveCenter(QPointF((-(width/2)+radius),((height/2)-radius)));

    QRectF horizontalFace(-((width)/2),-((height-(2*radius))/2),width,(height-(2*radius)));
    QRectF verticalFace(-((width-(2*radius))/2),-((height)/2),(width-(2*radius)),height);

    QPainterPath mask;
    QRectF maskEllipse;
    maskEllipse = _hole;
    maskEllipse.moveCenter(_holeCenter);
    mask.addEllipse(maskEllipse);
    mask = scene.subtracted(mask);

    painter->setClipPath(mask);

    painter->setPen(_drawingPen);
    painter->setBrush(_drawingBrush);
    painter->drawEllipse(topLeftCorner);
    painter->drawEllipse(topRightCorner);
    painter->drawEllipse(bottomRightCorner);
    painter->drawEllipse(bottomLeftCorner);
    painter->drawRect(horizontalFace);
    painter->drawRect(verticalFace);

}

void HollowRoundedGraphicsRectItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    _rect.setX(x);
    _rect.setY(y);
    _rect.setWidth(w);
    _rect.setHeight(h);
}

void HollowRoundedGraphicsRectItem::setRect(QRectF rect)
{
    _rect.setX(rect.x());
    _rect.setY(rect.y());
    _rect.setWidth(rect.width());
    _rect.setHeight(rect.height());
}

void HollowRoundedGraphicsRectItem::setHollow(bool hollow)
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

bool HollowRoundedGraphicsRectItem::isHollow()
{
    return _isHollow;
}

bool HollowRoundedGraphicsRectItem::hollow()
{
    return _isHollow;
}

void HollowRoundedGraphicsRectItem::setHoleCenterPos(qreal x, qreal y)
{
    _holeCenter.setX(x);
    _holeCenter.setY(-y);
}

void HollowRoundedGraphicsRectItem::setHoleCenterPos(QPointF point)
{
    _holeCenter.setX(point.x());
    _holeCenter.setY(-point.y());
}

void HollowRoundedGraphicsRectItem::setHoleSize(qreal diameter)
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

void HollowRoundedGraphicsRectItem::setHoleWidth(qreal width)
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

void HollowRoundedGraphicsRectItem::setHoleHeight(qreal height)
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

void HollowRoundedGraphicsRectItem::setHoleRect(QRectF holebb)
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

QRectF HollowRoundedGraphicsRectItem::holeSize()
{
    return _hole;
}

qreal HollowRoundedGraphicsRectItem::holeHeight()
{
    return _hole.height();
}

qreal HollowRoundedGraphicsRectItem::holeWidth()
{
    return _hole.width();
}

QPointF HollowRoundedGraphicsRectItem::getHoleCenter()
{
    return _holeCenter;
}

void HollowRoundedGraphicsRectItem::setBrush(QBrush brush)
{
    _drawingBrush = brush;
}

void HollowRoundedGraphicsRectItem::setBrush(QColor color)
{
    _drawingBrush.setColor(color);
}

void HollowRoundedGraphicsRectItem::setPen(QColor color)
{
    _drawingPen.setColor(color);
}

void HollowRoundedGraphicsRectItem::setPen(QPen pen)
{
    _drawingPen = pen;
}

void HollowRoundedGraphicsRectItem::setRoundness(int percentage)
{
    _roundness = percentage;
}

void HollowRoundedGraphicsRectItem::getMaximumCornerRadius()
{
    double shorterSide = _rect.width();
    if (_rect.height() < shorterSide) {
        shorterSide = _rect.height();
    }
    _maxCornerRadius = shorterSide/2;
}


