#include "hollowgraphicsrectitem.h"


QRectF HollowGraphicsRectItem::boundingRect() const
{
    return _rect;
}

void HollowGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
{
    std::ignore = option;
    std::ignore = widget;

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
    painter->drawRect(_rect);
}

void HollowGraphicsRectItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    _rect.setX(x);
    _rect.setY(y);
    _rect.setWidth(w);
    _rect.setHeight(h);
}

void HollowGraphicsRectItem::setRect(QRectF rect)
{
    _rect.setX(rect.x());
    _rect.setY(rect.y());
    _rect.setWidth(rect.width());
    _rect.setHeight(rect.height());
}

void HollowGraphicsRectItem::setHollow(bool hollow)
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

bool HollowGraphicsRectItem::isHollow()
{
    return _isHollow;
}

bool HollowGraphicsRectItem::hollow()
{
    return _isHollow;
}

void HollowGraphicsRectItem::setHoleCenterPos(qreal x, qreal y)
{
    _holeCenter.setX(x);
    _holeCenter.setY(-y);
}

void HollowGraphicsRectItem::setHoleCenterPos(QPointF point)
{
    _holeCenter.setX(point.x());
    _holeCenter.setY(-point.y());
}

void HollowGraphicsRectItem::setHoleSize(qreal diameter)
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

void HollowGraphicsRectItem::setHoleWidth(qreal width)
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

void HollowGraphicsRectItem::setHoleHeight(qreal height)
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

void HollowGraphicsRectItem::setHoleRect(QRectF holebb)
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

QRectF HollowGraphicsRectItem::holeSize()
{
    return _hole;
}

qreal HollowGraphicsRectItem::holeHeight()
{
    return _hole.height();
}

qreal HollowGraphicsRectItem::holeWidth()
{
    return _hole.width();
}

QPointF HollowGraphicsRectItem::getHoleCenter()
{
    return _holeCenter;
}

void HollowGraphicsRectItem::setBrush(QBrush brush)
{
    _drawingBrush = brush;
}

void HollowGraphicsRectItem::setBrush(QColor color)
{
    _drawingBrush.setColor(color);
}

void HollowGraphicsRectItem::setPen(QColor color)
{
    _drawingPen.setColor(color);
}

void HollowGraphicsRectItem::setPen(QPen pen)
{
    _drawingPen = pen;
}


