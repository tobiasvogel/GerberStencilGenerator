#include "snapslider.h"
#include <QDebug>

snapSlider::snapSlider( QWidget *parent ) {
   std::ignore = parent;
   QObject::connect( this, SIGNAL( valueChanged( int ) ), this, SLOT( currentValueChanged( int ) ) );
}

void snapSlider::keyPressEvent( QKeyEvent *ev ) {
   if ( ev->key() == Qt::Key_Shift ) {
      setSnapon();
      Q_EMIT snapOn( true );
   }

   ev->accept();
}

void snapSlider::keyReleaseEvent( QKeyEvent *ev ) {
   if ( _snapMode == true && ev->key() == Qt::Key_Shift ) {
      Q_EMIT snapOn( false );
      setSnapoff();
   }

   ev->accept();
}

void snapSlider::focusOutEvent( QFocusEvent *event ) {
   if ( _snapMode ) {
      Q_EMIT snapOn( false );
      setSnapoff();
   }

   event->accept();
}

void snapSlider::setSnapon() {
   _snapMode = true;
   float rVal = static_cast<float>( this->value() ) / static_cast<float>( _snapFactor );
   this->setMaximum( _initialMax / _snapFactor );
   this->setMinimum( _initialMin / _snapFactor );
   this->setValue( qRound( rVal ) );

   if ( this->tickPosition() != QSlider::NoTicks ) {
      this->setTickInterval( this->tickInterval() / _snapFactor );
   }
}

void snapSlider::setSnapoff() {
   _snapMode = false;
   this->setMaximum( _initialMax );
   this->setMinimum( _initialMin );
   this->setValue( this->value()*_snapFactor );

   if ( this->tickPosition() != QSlider::NoTicks ) {
      this->setTickInterval( _tickInterval );
   }
}

void snapSlider::setSnapping( int minimum, int maximum, int step ) {
   this->setMinimum( minimum );
   _initialMin = minimum;
   this->setMaximum( maximum );
   _initialMax = maximum;
   this->_snapFactor = step;
   _tickInterval = this->tickInterval();
}

void snapSlider::setSliderValue( int value ) {
   if ( !_snapMode ) {
      this->setValue( value );

   } else {
      float val = static_cast<float>( this->value() ) / static_cast<float>( _snapFactor );
      this->setValue( qRound( val ) );
   }
}

void snapSlider::currentValueChanged( int value ) {
   if ( !_snapMode ) {
      Q_EMIT snapValueChanged( value );

   } else {
      Q_EMIT snapValueChanged( value * _snapFactor );
   }
}
