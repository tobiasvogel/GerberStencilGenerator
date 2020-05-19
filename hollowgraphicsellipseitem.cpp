#include "hollowgraphicsellipseitem.h"

QRectF HollowGraphicsEllipseItem::boundingRect() const
{
    return _rect;
}

void HollowGraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawEllipse(_rect);
}

void HollowGraphicsEllipseItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    _rect.setX(x);
    _rect.setY(y);
    _rect.setWidth(w);
    _rect.setHeight(h);
}

void HollowGraphicsEllipseItem::setRect(QRectF rect)
{
    _rect.setX(rect.x());
    _rect.setY(rect.y());
    _rect.setWidth(rect.width());
    _rect.setHeight(rect.height());
}

void HollowGraphicsEllipseItem::setSize(qreal diameter)
{
    _rect.setX(-(diameter/2));
    _rect.setY(-(diameter/2));
    _rect.setWidth(diameter);
    _rect.setHeight(diameter);
}

void HollowGraphicsEllipseItem::setHollow(bool hollow)
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

bool HollowGraphicsEllipseItem::isHollow()
{
    return _isHollow;
}

bool HollowGraphicsEllipseItem::hollow()
{
    return _isHollow;
}

void HollowGraphicsEllipseItem::setHoleCenterPos(qreal x, qreal y)
{
    _holeCenter.setX(x);
    _holeCenter.setY(-y);
}

void HollowGraphicsEllipseItem::setHoleCenterPos(QPointF point)
{
    _holeCenter.setX(point.x());
    _holeCenter.setY(-point.y());
}

void HollowGraphicsEllipseItem::setHoleSize(qreal diameter)
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

void HollowGraphicsEllipseItem::setHoleWidth(qreal width)
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

void HollowGraphicsEllipseItem::setHoleHeight(qreal height)
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

void HollowGraphicsEllipseItem::setHoleRect(QRectF holebb)
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

QRectF HollowGraphicsEllipseItem::holeSize()
{
    return _hole;
}

qreal HollowGraphicsEllipseItem::holeHeight()
{
    return _hole.height();
}

qreal HollowGraphicsEllipseItem::holeWidth()
{
    return _hole.width();
}

QPointF HollowGraphicsEllipseItem::getHoleCenter()
{
    return _holeCenter;
}

void HollowGraphicsEllipseItem::setBrush(QBrush brush)
{
    _drawingBrush = brush;
}

void HollowGraphicsEllipseItem::setBrush(QColor color)
{
    _drawingBrush.setColor(color);
}

void HollowGraphicsEllipseItem::setPen(QColor color)
{
    _drawingPen.setColor(color);
}

void HollowGraphicsEllipseItem::setPen(QPen pen)
{
    _drawingPen = pen;
}

QStringList HollowGraphicsEllipseItem::getApertureMacro()
{
    QStringList apMacro;


    return apMacro;
}
