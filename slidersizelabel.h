#ifndef SLIDERSIZELABEL_H
#define SLIDERSIZELABEL_H
#include <QLabel>
#include <QMouseEvent>

class sliderSizeLabel : public QLabel {
public:
   sliderSizeLabel( QWidget *parent = nullptr );
   ~sliderSizeLabel();


protected:
   void mouseDoubleClickEvent( QMouseEvent *event );

   friend class GerberStencilGenerator;

protected Q_SLOTS:
   void disableControl();
   void enableControl();

   void enable( bool toggle );

   bool getEnabled();

private:
   QString notAvailableAbbreviation = tr( "n/a" );

   bool _isEnabled = true;

};

#endif // SLIDERSIZELABEL_H
