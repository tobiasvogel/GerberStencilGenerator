#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include <QSize>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "color.h"
#include "gerbv.h"

class RenderThread : public QThread {
   Q_OBJECT
public:
   RenderThread( QObject *parent = nullptr );
   ~RenderThread();

   void render( int width, int height, bool overlay, std::string file1, std::string file2, QColor lnColor, QColor flColor, QColor bgColor );

Q_SIGNALS:
   void renderedImage( const QImage &image, const QSize &size );

protected:
   void run() override;

private:

   gerbv_project_t *mainProject;
   gerbv_render_info_t screenRenderInfo;

   QMutex mutex;
   QWaitCondition condition;
   QSize size;
   bool abort = false;
   bool restart = false;
   bool _overlay;
   std::string _file1;
   std::string _file2;
   QColor _lnColor;
   QColor _flColor;
   QColor _bgColor;
};

#endif // RENDERTHREAD_H
