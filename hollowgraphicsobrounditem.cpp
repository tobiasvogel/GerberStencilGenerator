#include "hollowgraphicsobrounditem.h"
#include <QDebug>

QRectF HollowGraphicsObroundItem::boundingRect() const
{
    return _rect;
}

void HollowGraphicsObroundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    std::ignore = option;
    std::ignore = widget;

    this->calculateConsistingShapes();

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

    painter->drawPie(_circleA,(90*16), (180*16));
    painter->drawPie(_circleB, (90*16),-(180*16));
    painter->drawRect(_midSection);
}

void HollowGraphicsObroundItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    _rect.setX(x);
    _rect.setY(y);
    _rect.setWidth(w);
    _rect.setHeight(h);
}

void HollowGraphicsObroundItem::setRect(QRectF rect)
{
    _rect.setX(rect.x());
    _rect.setY(rect.y());
    _rect.setWidth(rect.width());
    _rect.setHeight(rect.height());
}

void HollowGraphicsObroundItem::setSize(qreal diameter)
{
    _rect.setX(-(diameter/2));
    _rect.setY(-(diameter/2));
    _rect.setWidth(diameter);
    _rect.setHeight(diameter);
}

void HollowGraphicsObroundItem::setWidth(qreal width)
{
    _rect.setX(-(width/2));
    _rect.setWidth(width);
}

void HollowGraphicsObroundItem::setHeight(qreal height)
{
    _rect.setY(-(height/2));
    _rect.setHeight(height);
}

void HollowGraphicsObroundItem::setHollow(bool hollow)
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

bool HollowGraphicsObroundItem::isHollow()
{
    return _isHollow;
}

bool HollowGraphicsObroundItem::hollow()
{
    return _isHollow;
}

void HollowGraphicsObroundItem::setHoleCenterPos(qreal x, qreal y)
{
    _holeCenter.setX(x);
    _holeCenter.setY(-y);
}

void HollowGraphicsObroundItem::setHoleCenterPos(QPointF point)
{
    _holeCenter.setX(point.x());
    _holeCenter.setY(-point.y());
}

void HollowGraphicsObroundItem::setHoleSize(qreal diameter)
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

void HollowGraphicsObroundItem::setHoleWidth(qreal width)
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

void HollowGraphicsObroundItem::setHoleHeight(qreal height)
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

void HollowGraphicsObroundItem::setHoleRect(QRectF holebb)
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

QRectF HollowGraphicsObroundItem::holeSize()
{
    return _hole;
}

qreal HollowGraphicsObroundItem::holeHeight()
{
    return _hole.height();
}

qreal HollowGraphicsObroundItem::holeWidth()
{
    return _hole.width();
}

QPointF HollowGraphicsObroundItem::getHoleCenter()
{
    return _holeCenter;
}

void HollowGraphicsObroundItem::setBrush(QBrush brush)
{
    _drawingBrush = brush;
}

void HollowGraphicsObroundItem::setBrush(QColor color)
{
    _drawingBrush.setColor(color);
}

void HollowGraphicsObroundItem::setPen(QColor color)
{
    _drawingPen.setColor(color);
}

void HollowGraphicsObroundItem::setPen(QPen pen)
{
    _drawingPen = pen;
}

QStringList HollowGraphicsObroundItem::getApertureMacro(QRectF boundingBox, QRectF holeSize, QPointF centerPoint, QPointF holeCenter, int roundness = 0, int vertices = 0, bool isHollow = false)
{
    std::ignore = roundness;
    std::ignore = vertices;

    QStringList apMacro;


    return apMacro;
}

double HollowGraphicsObroundItem::getShorterSide()
{
    if (_rect.width() < _rect.height()) {
        return _rect.width();
    } else {
        return _rect.height();
    }
}

double HollowGraphicsObroundItem::getLongerSide()
{
    if (_rect.width() > _rect.height()) {
        return _rect.width();
    } else {
        return _rect.height();
    }
}

void HollowGraphicsObroundItem::determineSideRatio()
{
    if (_rect.width() < _rect.height()) {
        _shorterSide = WIDTH;
    } else {
        if (_rect.width() == _rect.height()) {
            _shorterSide = EQUAL;
        } else {
            _shorterSide = HEIGHT;
        }
    }
    if (_rect.width() > _rect.height()) {
        _longerSide = WIDTH;
    } else {
        if (_rect.width() == _rect.height()) {
            _longerSide = EQUAL;
        } else {
            _longerSide = HEIGHT;
        }
    }
}

void HollowGraphicsObroundItem::calculateConsistingShapes()
{
    this->determineSideRatio();
    if (_shorterSide == WIDTH) {
        _midSection.setRect((-(getShorterSide()/2)),(-(getLongerSide()/2)+(getShorterSide()/2)),getShorterSide(),(getLongerSide()-getShorterSide()));
        _circleA.setRect(-(getShorterSide()/2),-(getLongerSide()/2),getShorterSide(),getShorterSide());
        _circleB.setRect(-(getShorterSide()/2),(getLongerSide()/2)-getShorterSide(),getShorterSide(),getShorterSide());
    } else if (_shorterSide == HEIGHT) {
        _midSection.setRect((-(getLongerSide()/2)+(getShorterSide()/2)),(-(getShorterSide()/2)),(getLongerSide()-getShorterSide()),getShorterSide());
        _circleA.setRect(_rect.x(),_rect.y(),getShorterSide(),getShorterSide());
        _circleB.setRect((getLongerSide()/2)-getShorterSide(),_rect.y(),getShorterSide(),getShorterSide());
    } else {
        _circleA.setRect(-(getShorterSide()/2),-(getShorterSide()/2),getShorterSide(),getShorterSide());
        _circleA.setRect(-(getShorterSide()/2),-(getShorterSide()/2),getShorterSide(),getShorterSide());
        _midSection.setRect(0,0,0,0);
    }
    qDebug() << "Rect:       " << _rect;
    qDebug() << "Circle A:   " << _circleA;
    qDebug() << "Circle B:   " << _circleB;
    qDebug() << "MidSection: " << _midSection;
}
