#include "apertureeditview.h"
#include "gerberstencilgenerator.h"
#include <QDebug>

apertureEditView::apertureEditView(QWidget *parent) {
    _lastWidth = this->getBoundingBox().width();
    _lastHeight = this->getBoundingBox().height();
}

apertureEditView::~apertureEditView() {

}


void apertureEditView::resizeEvent(QResizeEvent *event) {
        event->accept();
    if (this->items().count() == 0) {
        return;
    } else {
        this->adjustPreviewSize();
    }
}

void apertureEditView::adjustPreviewSize()
{
    if (this->items().count() == 0) {
        return;
    }

    this->adjustPreviewSize(_lastWidth, _lastHeight);
}

void apertureEditView::adjustPreviewSize(qreal width, qreal height)
{
    if (this->items().count() == 0) {
        return;
    }

    if (_lastWidth != width && width != 0) {
        _lastWidth = width;
    }
    if (_lastHeight != height && height != 0) {
        _lastHeight = height;
    }

    double maxHeight = height;
    double maxWidth = width;

    maxHeight *= 1.75;
    maxWidth *= 1.75;

    QRectF viewportBoundingBox;
    viewportBoundingBox.setWidth(maxWidth);
    viewportBoundingBox.setHeight(maxHeight);
    viewportBoundingBox.setX(-(maxWidth/2));
    viewportBoundingBox.setY(-(maxHeight/2));

    this->setBoundingBox(viewportBoundingBox);

    this->fitInView(viewportBoundingBox, Qt::KeepAspectRatio);
    this->centerOn(0,0);
    this->repaint();
    this->update();
}

void apertureEditView::setBoundingBoxHeight(qreal height){
    _boundingBox.setY(-(height/2));
    _boundingBox.setHeight(height);

}

void apertureEditView::setBoundingBoxWidth(qreal width){
    _boundingBox.setX(-(width/2));
    _boundingBox.setWidth(width);

}

void apertureEditView::setBoundingBoxSize(QRectF bbrect){
    _boundingBox.setWidth(bbrect.width());
    _boundingBox.setHeight(bbrect.height());
}

void apertureEditView::setBoundingBoxSize(qreal width, qreal height) {
    _boundingBox.setWidth(width);
    _boundingBox.setHeight(height);
}

void apertureEditView::setBoundingBox(QRectF bbrect)
{
    _boundingBox = bbrect;
}

QPointF apertureEditView::getShapeOffset()
{
    QRectF shapeBoundingBox = _boundingBox;
    QRectF sceneBoundingBox = this->scene()->itemsBoundingRect();
    QPointF shapeCenter = shapeBoundingBox.center();
    QPointF sceneCenter = sceneBoundingBox.center();
    return QPointF((sceneCenter.x()-shapeCenter.x()),(sceneCenter.y()-shapeCenter.y()));
}

const QRectF apertureEditView::getBoundingBox() {
    return _boundingBox;
}
