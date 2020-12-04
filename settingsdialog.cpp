#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "gerberstencilgenerator.h"
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QIcon>
#include <QMovie>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QXmlSimpleReader>
#include <QDomDocument>
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonObject>

#if defined(Q_OS_WIN)
   #define PLATFORM "win"
#elif defined(Q_OS_MAC)
   #define PLATFORM "mac"
#elif defined(Q_OS_LINUX)
   #define PLATFORM "linux"
#else
   #define PLATFORM "unknown"
#endif


settingsDialog::settingsDialog( GerberStencilGenerator *parent ) : QDialog( parent ), ui( new Ui::settingsDialog ) {
   ui->setupUi( this );
   gsgInstance = parent;
   QObject::connect( ui->backgroundColorChooser, SIGNAL( clicked() ), this, SLOT( selectNewColor() ) );
   QObject::connect( ui->adjustmentColorChooser, SIGNAL( clicked() ), this, SLOT( selectNewColor() ) );
   QObject::connect( ui->flashColorChooser, SIGNAL( clicked() ), this, SLOT( selectNewColor() ) );
   QObject::connect( ui->lineColorChooser, SIGNAL( clicked() ), this, SLOT( selectNewColor() ) );
}

settingsDialog::~settingsDialog() {
   gsgInstance = nullptr;
}

void settingsDialog::selectNewColor() {
   QString colorName = this->sender()->objectName();

   if ( colorName.endsWith( "Chooser" ) ) {
      colorName = colorName.left( colorName.length() - 7 );

   } else {
      return;
   }

   QColor initialColor;
   initialColor = QColor( gsgInstance->getColor( colorName ) );
   QColor newColor;
   newColor = gsgInstance->pickColor( initialColor, tr( "Select Color" ) );
   gsgInstance->setColor( colorName, newColor );
   this->refreshColors();
}

void settingsDialog::refreshColors() {
   ui->backgroundColorFrame->setStyleSheet( QString( "background-color: %1;" ).arg( gsgInstance->getColor( "backgroundColor" ).name() ) );
   ui->adjustmentColorFrame->setStyleSheet( QString( "background-color: %1;" ).arg( gsgInstance->getColor( "adjustmentColor" ).name() ) );
   ui->flashColorFrame->setStyleSheet( QString( "background-color: %1;" ).arg( gsgInstance->getColor( "flashColor" ).name() ) );
   ui->lineColorFrame->setStyleSheet( QString( "background-color: %1;" ).arg( gsgInstance->getColor( "lineColor" ).name() ) );
}
