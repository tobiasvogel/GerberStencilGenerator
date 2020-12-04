#ifndef COLOR_H
#define COLOR_H

#include <gtk/gtk.h>
#include <stdint.h>
#include <QColor>
#include <QObject>


class Gtk_Extern_Color {
public:
   Gtk_Extern_Color( QColor color ) {
      int r_value = color.red() << 8;
      int g_value = color.green() << 8;
      int b_value = color.blue() << 8;
      int a_value = color.alpha() << 8;
      _r = static_cast<uint16_t>( color.red() ) + static_cast<uint16_t>( r_value );
      _g = static_cast<uint16_t>( color.green() ) + static_cast<uint16_t>( g_value );
      _b = static_cast<uint16_t>( color.blue() ) + static_cast<uint16_t>( b_value );
      _a = static_cast<uint16_t>( color.alpha() ) + static_cast<uint16_t>( a_value );
   }

   guint16 r( void ) { return _r; }
   guint16 g( void ) { return _g; }
   guint16 b( void ) { return _b; }
   guint16 a( void ) { return _a; }

private:
   uint16_t _r = 0;
   uint16_t _g = 0;
   uint16_t _b = 0;
   uint16_t _a = 0;

};

class Cairo_Extern_Color {
public:
   Cairo_Extern_Color( QColor color ) {
      double r_value = static_cast<double>( color.red() );
      double g_value = static_cast<double>( color.green() );
      double b_value = static_cast<double>( color.blue() );
      double a_value = static_cast<double>( color.alpha() );
      _r = r_value / 255.000;
      _g = g_value / 255.000;
      _b = b_value / 255.000;
      _a = a_value / 255.000;
   }

   double r( void ) { return _r; }
   double g( void ) { return _g; }
   double b( void ) { return _b; }
   double a( void ) { return _a; }

private:
   double _r = 0.0;
   double _g = 0.0;
   double _b = 0.0;
   double _a = 0.0;
};


class Color {
public:
   Color( QColor color ) {
      _gtk = new Gtk_Extern_Color( color );
      _cairo = new Cairo_Extern_Color( color );
   }

   Gtk_Extern_Color *gtk( void ) { return _gtk; }
   Cairo_Extern_Color *cairo( void ) { return _cairo; }
   QColor *qcolor( void ) { return _qcolor; }

private:
   Gtk_Extern_Color *_gtk;
   Cairo_Extern_Color *_cairo;
   QColor *_qcolor;
};



#endif // COLOR_H
