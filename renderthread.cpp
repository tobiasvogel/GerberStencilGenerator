#include "renderthread.h"

RenderThread::RenderThread( QObject *parent ) : QThread( parent ) {
   screenRenderInfo.renderType = GERBV_RENDER_TYPE_CAIRO_HIGH_QUALITY;
}

RenderThread::~RenderThread() {
   mutex.lock();
   abort = true;
   gerbv_destroy_project( mainProject );
   condition.wakeOne();
   mutex.unlock();
   wait();
}

void RenderThread::render( int width, int height, bool overlay, std::string file1, std::string file2, QColor lnColor,
                           QColor flColor, QColor bgColor ) {
   QMutexLocker locker( &mutex );

   size = QSize( width, height );
   _overlay = overlay;
   _file1 = file1;
   _file2 = file2;
   _lnColor = lnColor;
   _flColor = flColor;
   _bgColor = bgColor;

   mainProject = gerbv_create_project();

   if ( !isRunning() ) {
      start( LowPriority );

   } else {
      restart = true;
      condition.wakeOne();
   }
}

void RenderThread::run() {

   screenRenderInfo.displayWidth = size.width();
   screenRenderInfo.displayHeight = size.height();
   Color lnColor = Color( _lnColor );
   Color flColor = Color( _flColor );
   Color bgColor = Color( _bgColor );

   cairo_surface_t *target = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, size.width(), size.height() );
   cairo_t *cr = cairo_create( target );

   gerbv_open_layer_from_filename_with_color( mainProject, const_cast<gchar *>( _file1.c_str() ), lnColor.gtk()->r(),
         lnColor.gtk()->g(), lnColor.gtk()->b(), lnColor.gtk()->a() );

   if ( _overlay ) {
      gerbv_open_layer_from_filename_with_color( mainProject, const_cast<gchar *>( _file2.c_str() ), flColor.gtk()->r(),
            flColor.gtk()->g(), flColor.gtk()->b(), flColor.gtk()->a() );
   }

   if ( ( mainProject->file[0] == nullptr ) || ( ( _overlay == true ) && ( mainProject->file[1] == nullptr ) ) ) {
      qDebug() << "There was an error parsing the files.";
   }

   gerbv_render_all_layers_to_cairo_target( mainProject, cr, &screenRenderInfo );

   // paint the background before we draw anything
   cairo_set_source_rgba( cr, bgColor.cairo()->r(), bgColor.cairo()->g(), bgColor.cairo()->b(), 1 );
   cairo_paint( cr );

   gerbv_render_zoom_to_fit_display( mainProject, &screenRenderInfo );

   //layer 0
   if ( mainProject->file[0] ) {
      cairo_push_group( cr );
      gerbv_render_layer_to_cairo_target( cr, mainProject->file[0], &screenRenderInfo );
      cairo_pop_group_to_source( cr );
      cairo_paint( cr );
   }

   //layer 1
   if ( mainProject->file[1] ) {
      cairo_push_group( cr );
      gerbv_render_layer_to_cairo_target( cr, mainProject->file[1], &screenRenderInfo );
      cairo_pop_group_to_source( cr );
      cairo_paint( cr );
   }

   cairo_surface_flush( target );

   if ( !restart ) {
      Q_EMIT renderedImage( QImage( cairo_image_surface_get_data( target ), size.width(), size.height(),
                                    QImage::Format_ARGB32 ), size );
      cairo_destroy( cr );
      gerbv_destroy_project( mainProject );

   } else {
      cairo_destroy( cr );
      gerbv_destroy_project( mainProject );
   }

   //gerbv_destroy_project (mainProject);
   mutex.lock();

   if ( !restart ) {
      condition.wait( &mutex );
   }

   restart = false;
   mutex.unlock();
}
