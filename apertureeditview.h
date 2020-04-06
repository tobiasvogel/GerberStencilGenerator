#ifndef APERTUREEDITVIEW_H
#define APERTUREEDITVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class apertureEditView : public QGraphicsView {
public:
    apertureEditView(QWidget *parent = nullptr);
    ~apertureEditView();

protected:
    void resizeEvent(QResizeEvent *event);
    void adjustPreviewSize();
    void adjustPreviewSize(qreal width, qreal height);
    void setBoundingBoxHeight(qreal height);
    void setBoundingBoxWidth(qreal width);
    void setBoundingBoxSize(QRectF bbrect);
    void setBoundingBoxSize(qreal width, qreal height);
    void setBoundingBox(QRectF bbrect);
    QPointF getShapeOffset(void);
    const QRectF getBoundingBox(void);


private:
    QRectF _boundingBox;
    qreal _lastWidth = 0;
    qreal _lastHeight = 0;


    friend class GerberStencilGenerator;
};


#endif // APERTUREEDITVIEW_H
