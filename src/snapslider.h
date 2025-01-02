#ifndef SNAPSLIDER_H
#define SNAPSLIDER_H

#include <QSlider>
#include <QKeyEvent>
#include <QFocusEvent>

class snapSlider : public QSlider {
   Q_OBJECT
public:
   snapSlider( QWidget *parent );

public Q_SLOTS:
   void setSnapping( int minimum, int maximum, int step );
   void setSliderValue( int value );

private Q_SLOTS:
   void currentValueChanged( int value );

Q_SIGNALS:
   void snapOn( bool );
   void snapValueChanged( int );


protected:
   void keyPressEvent( QKeyEvent *ev );
   void keyReleaseEvent( QKeyEvent *ev );
   void focusOutEvent( QFocusEvent *event );
   void setSnapon( void );
   void setSnapoff( void );


private:
   bool _snapMode = false;
   signed int _initialMax;
   signed int _initialMin;
   int _snapFactor = 10;
   int _tickInterval = 10;
};

#endif // SNAPSLIDER_H
