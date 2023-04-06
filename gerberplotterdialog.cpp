#include "gerberplotterdialog.h"
#include "ui_gerberplotterdialog.h"


GerberPlotterDialog::GerberPlotterDialog( QWidget *parent ) :
   QDialog( parent ),
   ui( new Ui::GerberPlotterDialog ) {
   ui->setupUi( this );

   colorDialog = new color_widgets::ColorDialog( this );

   lastDir = QDir::home();
   QObject::connect( ui->cancelButton, &QPushButton::clicked, [this]() { close(); } );
   //QObject::connect(ui->generateButton, &QPushButton::clicked, [this]() { std::ignore = this; qDebug() << "Not implemented yet."; });
   QObject::connect( ui->boardOutlineBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->boardOutlineLineEdit, tr( "Board Outline Gerber File" ), nullptr, tr( "Board Outline (*.gml);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->drillsBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->drillsLineEdit, tr( "Drills Gerber File" ), nullptr,
                      tr( "Excellon Drills File (*.xln *.txt *.drl *.nc *.exc *.drd);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->topCopperBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->topCopperLineEdit, tr( "Top Copper Gerber File" ), nullptr,
                      tr( "Copper Gerber File (*.gtl *.top *.cmp);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->topSolderMaskBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->topSolderMaskLineEdit, tr( "Top Solder Mask Gerber File" ), nullptr,
                      tr( "Solder Mask Gerber File (*.gts *.tsm *.smt);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->topSilkscreenBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->topSilkscreenLineEdit, tr( "Top Silkscreen Gerber File" ), nullptr,
                      tr( "Silkscreen Gerber File (*.gto *.tsk *.sst);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->bottomCopperBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->bottomCopperLineEdit, tr( "Bottom Copper Gerber File" ), nullptr,
                      tr( "Copper Gerber File (*.gbl *.bot *.sol);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->bottomSolderMaskBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->bottomSolderMaskLineEdit, tr( "Bottom Solder Mask Gerber File" ), nullptr,
                      tr( "Solder Mask Gerber File (*.gbs *.bsm *.smb);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->bottomSilkscreenBrowseButton, &QPushButton::clicked, [this]() {
      browseOpenFile( ui->bottomSilkscreenLineEdit, tr( "Bottom Silkscreen Gerber File" ), nullptr,
                      tr( "Silkscreen Gerber File (*.gbo *.bsk *.ssb);;Generic Gerber File (*.grb);;Any file (*.*)" ) );
   } );
   QObject::connect( ui->topSideBrowseButton, &QPushButton::clicked, [this]() {
      checkImageFormat();

      if ( graphicsFormat == PNG ) {
         browseSaveFile( ui->topSideLineEdit, tr( "Top Side Output Image File" ), nullptr, tr( "PNG-Graphic (*.png)" ) );

      } else {
         browseSaveFile( ui->topSideLineEdit, tr( "Top Side Output Image File" ), nullptr, tr( "JPEG-Image (*.jpg *.jpeg)" ) );
      }
   } );
   QObject::connect( ui->bottomSideBrowseButton, &QPushButton::clicked, [this]() {
      checkImageFormat();

      if ( graphicsFormat == PNG ) {
         browseSaveFile( ui->bottomSideLineEdit, tr( "Bottom Side Output Image File" ), nullptr, tr( "PNG-Graphic (*.png)" ) );

      } else {
         browseSaveFile( ui->bottomSideLineEdit, tr( "Bottom Side Output Image File" ), nullptr, tr( "JPEG-Image (*.jpg *.jpeg)" ) );
      }
   } );
   QObject::connect( ui->radioButton_jpeg, &QRadioButton::clicked, [this]() { imageFormatChanged(); } );
   QObject::connect( ui->radioButton_png, &QRadioButton::clicked, [this]() { imageFormatChanged(); } );
   QObject::connect( ui->radioButton_green, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->radioButton_blue, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->radioButton_red, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->radioButton_yellow, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->radioButton_white, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->radioButton_black, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->radioButton_custom, &QRadioButton::clicked, [this]() { colorSchemeChanged(); } );
   QObject::connect( ui->padsColorComboBox, QOverload<int>::of( &QComboBox::activated ),
   [ = ]( int index ) { padsColorChanged( index ); } );

   QObject::connect( ui->bgColorComboBox, QOverload<int>::of( &QComboBox::activated ),
   [ = ]( int index ) { backgroundColorChanged( index ); } );

   QObject::connect( ui->bottomSideOutputGroupBox, QOverload<bool>::of( &QGroupBox::toggled ),
   [ = ]( bool on ) { bottomSideOutputToggled( on ); } );

   QObject::connect( ui->topSideOutputGroupBox, QOverload<bool>::of( &QGroupBox::toggled ),
   [ = ]( bool on ) { topSideOutputToggled( on ); } );

   QObject::connect( ui->imgWidthBox, QOverload<int>::of( &QSpinBox::valueChanged ),
   [ = ]( int width ) { imgSizeChanged( width, -1 ); } );

   QObject::connect( ui->imgHeightBox, QOverload<int>::of( &QSpinBox::valueChanged ),
   [ = ]( int height ) { imgSizeChanged( -1, height ); } );

   QObject::connect( ui->evalBoardSizeButton, &QPushButton::clicked, [this]() { loadGerberBoardOutline(); } );




   disabledFieldStylesheet = "background-color: ";
   disabledFieldStylesheet.append( QColor( QPalette().color( QPalette::Disabled, QPalette::Base ) ).name( QColor::HexRgb ) );
   disabledFieldStylesheet.append( "; color: " );
   disabledFieldStylesheet.append( QColor( QPalette().color( QPalette::Disabled, QPalette::WindowText ) ).name( QColor::HexRgb ) );
   disabledFieldStylesheet.append( ";" );

   checkImageFormat();

   toggleSizeLoaded();
}

GerberPlotterDialog::~GerberPlotterDialog() {
   delete ui;
}

QString GerberPlotterDialog::colorComboBoxText( QString text, QColor color ) {
   QString returnText = text;

   if ( text.endsWith( ")" ) ) {
      // there is already a color-string appended
      if ( text.right( 8 ).startsWith( QChar( 0x0023 ) ) ) { // HexRgb
         returnText = text.left( text.size() - 10 );

      } else if ( text.right( 10 ).startsWith( QChar( 0x0023 ) ) ) { // HexArgb
         returnText = text.left( text.size() - 12 );

      } else {
         qDebug() << "uh-oh.. something went very wrong here :-(";
         return text;
      }
   }

   returnText.append( QChar( 0x0020 ) );
   returnText.append( QChar( 0x0028 ) );

   if ( color.alpha() == 255 ) {
      returnText.append( color.name( QColor::HexRgb ) );

   } else {
      returnText.append( color.name( QColor::HexArgb ) );
   }

   returnText.append( QChar( 0x0029 ) );
   return returnText;
}

QColor GerberPlotterDialog::getLastCustomColor( QString text, bool alpha, QColor initialColor = Qt::white ) {
   QColor returnColor = initialColor;
   QString colorString = "";

   if ( !( text.endsWith( ")" ) ) ) {
      // no custom color selected yet
      return returnColor;

   } else {
      if ( text.right( 8 ).startsWith( QChar( 0x0023 ) ) ) { // HexRgb
         colorString = text.right( 8 );
         colorString = colorString.left( colorString.size() - 1 );
         qDebug() << colorString;

         if ( QColor( colorString ).isValid() ) {
            returnColor = QColor( colorString );
         }

      } else if ( text.right( 10 ).startsWith( QChar( 0x0023 ) ) ) { // HexArgb
         colorString = text.right( 10 );
         colorString = colorString.left( colorString.size() - 1 );
         qDebug() << colorString;

         if ( QColor( colorString ).isValid() ) {
            returnColor = QColor( colorString );
         }

      } else {
         // something went wrong
         return returnColor;
      }
   }

   if ( !alpha ) {
      returnColor.setAlpha( 255 );
   }

   return returnColor;
}

void GerberPlotterDialog::browseOpenFile( QLineEdit *lineEdit, QString promptText, QString browseDir, QString fileFilters ) {
   QString dir;

   if ( browseDir.isNull() || browseDir.isEmpty() ) {
      dir = getLastDir();

   } else {
      dir = browseDir;
   }

   QString fileName = QFileDialog::getOpenFileName( this, promptText, dir, fileFilters );

   if ( fileName.isEmpty() || fileName.isNull() ) {
      return;

   } else {
      lineEdit->setText( fileName );
      QString filePath = QFileInfo( fileName ).absolutePath();
      lastDir.setPath( filePath );
   }
}

void GerberPlotterDialog::browseSaveFile( QLineEdit *lineEdit, QString promptText, QString browseDir, QString fileFilter ) {
   QString dir;

   if ( browseDir.isNull() || browseDir.isEmpty() ) {
      dir = getLastDir();

   } else {
      dir = browseDir;
   }

   QString fileName = QFileDialog::getSaveFileName( this, promptText, dir, fileFilter );

   if ( fileName.isEmpty() || fileName.isNull() ) {
      return;

   } else {
      lineEdit->setText( fileName );
      QString filePath = QFileInfo( fileName ).absolutePath();
      lastDir.setPath( filePath );
   }
}

void GerberPlotterDialog::colorSchemeChanged( void ) {

   if ( ui->radioButton_green->isChecked() ) { pcbColorScheme = PCB_GREEN; }

   else if ( ui->radioButton_blue->isChecked() ) { pcbColorScheme = PCB_BLUE; }

   else if ( ui->radioButton_red->isChecked() ) { pcbColorScheme = PCB_RED; }

   else if ( ui->radioButton_yellow->isChecked() ) { pcbColorScheme = PCB_YELLOW; }

   else if ( ui->radioButton_white->isChecked() ) { pcbColorScheme = PCB_WHITE; }

   else if ( ui->radioButton_black->isChecked() ) { pcbColorScheme = PCB_BLACK; }

   else if ( ui->radioButton_custom->isChecked() ) { pcbColorScheme = PCB_CUSTOM; }

   else { pcbColorScheme = PCB_GREEN; }

   QStringList colors;


   switch ( pcbColorScheme ) {
      case PCB_BLUE:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString::fromStdString( bluePcbGoldPad ).split( QLatin1Char( ';' ) );

         } else {
            colors =  QString::fromStdString( bluePcbSilverPad ).split( QLatin1Char( ';' ) );
         }

         break;

      case PCB_RED:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString::fromStdString( redPcbGoldPad ).split( QLatin1Char( ';' ) );

         } else {
            colors =  QString::fromStdString( redPcbSilverPad ).split( QLatin1Char( ';' ) );
         }

         break;

      case PCB_YELLOW:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString::fromStdString( yellowPcbGoldPad ).split( QLatin1Char( ';' ) );

         } else {
            colors =  QString::fromStdString( yellowPcbSilverPad ).split( QLatin1Char( ';' ) );
         }

         break;

      case PCB_WHITE:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString::fromStdString( whitePcbGoldPad ).split( QLatin1Char( ';' ) );

         } else {
            colors =  QString::fromStdString( whitePcbSilverPad ).split( QLatin1Char( ';' ) );
         }

         break;

      case PCB_BLACK:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString::fromStdString( blackPcbGoldPad ).split( QLatin1Char( ';' ) );

         } else {
            colors =  QString::fromStdString( blackPcbSilverPad ).split( QLatin1Char( ';' ) );
         }

         break;

      case PCB_CUSTOM:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString( customPcbGoldPad ).split( QLatin1Char( ';' ) );
            _padsColor = QColor( QString( "#" ).append( goldPcbPadColor.c_str() ) );

         } else {
            colors =  QString( customPcbSilverPad ).split( QLatin1Char( ';' ) );

            if ( ui->padsColorComboBox->currentIndex() != ( ui->padsColorComboBox->count() - 1 ) ) { // selected pad color must be silver
               _padsColor = QColor( QString( "#" ).append( silverPcbPadColor.c_str() ) );
            }
         }

         break;

      case PCB_GREEN:
      default:
         if ( ui->padsColorComboBox->currentText().toLower().contains( tr( "gold" ) ) ) {
            colors =  QString::fromStdString( greenPcbGoldPad ).split( QLatin1Char( ';' ) );

         } else {
            colors =  QString::fromStdString( greenPcbSilverPad ).split( QLatin1Char( ';' ) );
         }
   }

   ui->baseColorLineEdit->setText( QString( "#" ).append( colors.at( 0 ) ) );
   ui->copperColorLineEdit->setText( QString( "#" ).append( colors.at( 1 ) ) );
   ui->silkscreenColorLineEdit->setText( QString( "#" ).append( colors.at( 2 ) ) );
   ui->bumpMapSpinBox->setValue( colors.at( 4 ).toInt() );

}

void GerberPlotterDialog::imageFormatChanged() {
   if ( ui->radioButton_png->isChecked() ) {
      graphicsFormat = PNG;
      ui->jpgQualityBox->setEnabled( false );

      if ( ui->bgColorComboBox->count() < 4 ) {
         ui->bgColorComboBox->insertItem( 0, tr( "transparent" ) );
      }

   } else if ( ui->radioButton_jpeg->isChecked() ) {
      graphicsFormat = JPEG;
      ui->jpgQualityBox->setEnabled( true );

      if ( ui->bgColorComboBox->currentIndex() == 0 ) {
         ui->bgColorComboBox->setCurrentIndex( 2 );
      }

      if ( ui->bgColorComboBox->count() == 4 ) {
         ui->bgColorComboBox->removeItem( 0 );
      }

      if ( ui->bgColorComboBox->currentIndex() == ( ui->bgColorComboBox->count() - 1 ) ) {
         // previously selected color may contain alpha value, let the user re-pick
         backgroundColorChanged( ui->bgColorComboBox->currentIndex() );
      }

   } else {
      return;
   }
}

void GerberPlotterDialog::backgroundColorChanged( int index ) {
   if ( graphicsFormat == PNG ) {
      if ( index == 0 ) {
         _backgroundColor = Qt::transparent;

      } else if ( index == 1 ) {
         _backgroundColor = Qt::black;

      } else if ( index == 2 ) {
         _backgroundColor = Qt::white;

      } else if ( index == 3 ) {
         _backgroundColor = this->pickColor( getLastCustomColor( ui->bgColorComboBox->currentText(), true ), tr( "Choose background Color" ), true );
         ui->bgColorComboBox->setItemText( index, colorComboBoxText( ui->bgColorComboBox->currentText(), _backgroundColor ) );

      } else {
         return;
      }

   } else { // graphicsFormat == JPEG
      if ( index == 0 ) {
         _backgroundColor = Qt::black;

      } else if ( index == 1 ) {
         _backgroundColor = Qt::white;

      } else if ( index == 2 ) {
         _backgroundColor = this->pickColor( getLastCustomColor( ui->bgColorComboBox->currentText(), false ), tr( "Choose background Color" ), false );
         ui->bgColorComboBox->setItemText( index, colorComboBoxText( ui->bgColorComboBox->currentText(), _backgroundColor ) );

      } else {
         return;
      }
   }
}

void GerberPlotterDialog::padsColorChanged( int index ) {
   if ( ( ui->padsColorComboBox->itemText( index ).contains( tr( "gold" ) ) ) || ( ui->padsColorComboBox->itemText( index ).contains( tr( "silver" ) ) ) ) {
      colorSchemeChanged();

   } else {
      _padsColor = this->pickColor( getLastCustomColor( ui->padsColorComboBox->currentText(), false, _padsColor ), tr( "Choose background Color" ), false );
      ui->padsColorComboBox->setItemText( index, colorComboBoxText( ui->padsColorComboBox->currentText(), _padsColor ) );
   }
}

QColor GerberPlotterDialog::pickColor( QColor initialColor, QString windowTitle, bool alpha ) {
   if ( alpha ) {
      colorDialog->setAlphaEnabled( true );

   } else {
      colorDialog->setAlphaEnabled( false );
   }

   colorDialog->setWindowTitle( windowTitle );
   colorDialog->setColor( initialColor );
   colorDialog->exec();
   return colorDialog->color();
}

QString GerberPlotterDialog::getLastDir() {
   QString lastDirString = lastDir.absolutePath();
   return lastDirString;
}

void GerberPlotterDialog::checkImageFormat() {
   if ( ui->radioButton_png->isChecked() ) {
      graphicsFormat = PNG;
      ui->jpgQualityBox->setEnabled( false );

   } else {
      graphicsFormat = JPEG;
      ui->jpgQualityBox->setEnabled( true );
   }
}

void GerberPlotterDialog::bottomSideOutputToggled( bool on ) {
   ui->flipBottomCheckBox->setEnabled( on );
}

void GerberPlotterDialog::topSideOutputToggled( bool on ) {
   std::ignore = on;
}

void GerberPlotterDialog::imgSizeChanged( int width, int height ) {
   qDebug() << "( " << width << ", " << height << ");";

   if ( ( ( width < 0 ) && ( height < 0 ) ) ) { // something very fishy is going on...
      {
         const QSignalBlocker firstSignalBlocker( ui->imgWidthBox );
         const QSignalBlocker secondSignalBlocker( ui->imgHeightBox );
         ui->imgWidthBox->setValue( 1600 );
      }
      this->imgSizeChanged( 1600, -1 );
   }

   if ( width < 0 ) { // height was changed
      double dWidth = height / imgSizeConstraint;
      int _width = qRound( dWidth );
      {
         const QSignalBlocker blocker( ui->imgWidthBox );
         ui->imgWidthBox->setValue( _width );
      }

   } else if ( height < 0 ) {
      double dHeight = width * imgSizeConstraint;
      int _height = qRound( dHeight );
      {
         const QSignalBlocker blocker( ui->imgHeightBox );
         ui->imgHeightBox->setValue( _height );
      }
   }

   return;
}

void GerberPlotterDialog::loadGerberBoardOutline( void ) {

   if ( !_outlineLoaded ) {
      QString filename = ui->boardOutlineLineEdit->text();

      if ( filename.isNull() || filename.isEmpty() ) { return; }

      if ( !( QFile( filename ).exists() ) ) {
         //FIXME: throw error
         return;
      }

      mainProject = gerbv_create_project();

      std::string _file1 = filename.toStdString();

      gerbv_open_layer_from_filename( mainProject, const_cast<gchar *>( _file1.c_str() ) );

      if ( !( mainProject->file[0] ) ) {
         //FIXME: throw error
         return;
      }

      gerbv_render_size_t *renderSize = new gerbv_render_size_t;

      gerbv_render_get_boundingbox( mainProject, renderSize );

      QRectF renderRect;
      renderRect.setTop( renderSize->top );
      renderRect.setLeft( renderSize->left );
      renderRect.setBottom( renderSize->bottom );
      renderRect.setRight( renderSize->right );

      if ( ( renderRect.width() <= 0 ) || ( renderRect.height() <= 0 ) ) {
         // FIXME: throw error
         return;
      }

      imgSizeConstraint = renderRect.height() / renderRect.width();

      gerbv_destroy_project( mainProject );

      imgSizeChanged( ui->imgWidthBox->value(), -1 );

      _outlineLoaded = true;

   } else {

      _outlineLoaded = false;
   }

   toggleSizeLoaded();
}

void GerberPlotterDialog::toggleSizeLoaded() {

   bool loaded = _outlineLoaded;
   ui->imageSizeGroupBox->setEnabled( loaded );
   ui->boardOutlineGroupBox->setEnabled( !loaded );
   ui->drillsGroupBox->setEnabled( loaded );
   ui->topCopperGroupBox->setEnabled( loaded );
   ui->topSilkscreenGroupBox->setEnabled( loaded );
   ui->topSolderMaskGroupBox->setEnabled( loaded );
   ui->bottomCopperGroupBox->setEnabled( loaded );
   ui->bottomSilkscreenGroupBox->setEnabled( loaded );
   ui->bottomSolderMaskGroupBox->setEnabled( loaded );
   ui->generateButton->setEnabled( loaded );

   if ( loaded ) {
      ui->evalBoardSizeButton->setText( tr( "Reset Board Outline" ) );

   } else {
      ui->evalBoardSizeButton->setText( tr( "Get Size from Outline" ) );
   }
}

QColor GerberPlotterDialog::getBackgroundColor() {
   QColor returnColor;
   returnColor = _backgroundColor;
   return returnColor;
}

QColor GerberPlotterDialog::getBaseColor() {
   QString colorCandidate = ui->baseColorLineEdit->text();
   QColor returnColor = QColor( colorCandidate );
   return returnColor;
}

QColor GerberPlotterDialog::getSoldermaskColor() {
   QString colorCandidate = ui->baseColorLineEdit->text();
   QColor returnColor = QColor( colorCandidate );
   return returnColor;
}

QColor GerberPlotterDialog::getCopperColor() {
   QString colorCandidate = ui->copperColorLineEdit->text();
   QColor returnColor = QColor( colorCandidate );
   return returnColor;
}

QColor GerberPlotterDialog::getSilkscreenColor() {
   QString colorCandidate = ui->silkscreenColorLineEdit->text();
   QColor returnColor = QColor( colorCandidate );
   return returnColor;
}

QColor GerberPlotterDialog::getPadsColor() {
   QColor returnColor;
   returnColor = _padsColor;
   return returnColor;
}

QColor GerberPlotterDialog::getAlphaMaskColor() {
   QString colorCandidate = "#ff00ff";
   QColor returnColor = QColor( colorCandidate );
   return returnColor;
}

void GerberPlotterDialog::generateImageFile( pcb_side pcbSide, bool flip ) {

   outlineImage = new QTemporaryFile( "outline.png.XXXXXX", this );
   soldermaskImage = new QTemporaryFile( "soldermask.png.XXXXXX", this );
   copperImage = new QTemporaryFile( "copper.png.XXXXXX", this );
   drillsImage = new QTemporaryFile( "drills.png.XXXXXX", this );
   silkscreenImage = new QTemporaryFile( "silkscreen.png.XXXXXX", this );

   outlineImage->setAutoRemove( false );
   soldermaskImage->setAutoRemove( false );
   copperImage->setAutoRemove( false );
   drillsImage->setAutoRemove( false );
   silkscreenImage->setAutoRemove( false );

   QColor backgroundColor = getBackgroundColor();
   QColor baseColor = getBaseColor();
   QColor soldermaskColor = getSoldermaskColor();
   QColor copperColor = getCopperColor();
   QColor silkscreenColor = getSilkscreenColor();
   QColor padsColor = getPadsColor();
   QColor alphaMaskColor = getAlphaMaskColor();

   int width = ui->imgWidthBox->value();
   int height = ui->imgHeightBox->value();


   QString outlineGerberFile = ui->boardOutlineLineEdit->text();
   QString soldermaskGerberFile;

   if ( pcbSide == PCB_TOP ) {
      soldermaskGerberFile = ui->topSolderMaskLineEdit->text();

   } else if ( pcbSide == PCB_BOTTOM ) {
      soldermaskGerberFile = ui->bottomSolderMaskLineEdit->text();
   }

   QString copperGerberFile;

   if ( pcbSide == PCB_TOP ) {
      copperGerberFile = ui->topCopperLineEdit->text();

   } else if ( pcbSide == PCB_BOTTOM ) {
      copperGerberFile = ui->bottomCopperLineEdit->text();
   }

   QString drillsGerberFile = ui->drillsLineEdit->text();
   QString silkscreenGerberFile;

   if ( pcbSide == PCB_TOP ) {
      silkscreenGerberFile = ui->topSilkscreenLineEdit->text();

   } else if ( pcbSide == PCB_BOTTOM ) {
      silkscreenGerberFile = ui->bottomSilkscreenLineEdit->text();
   }


   // make outline image
   outlineImage->open();
   outlineImage->close();
   renderImage( outlineGerberFile.toStdString(), outlineGerberFile.toStdString(), Color( baseColor ), outlineImage->fileName().toStdString(), width, height );


   // make soldermask (pads) image
   soldermaskImage->open();
   soldermaskImage->close();
   renderImage( soldermaskGerberFile.toStdString(), outlineGerberFile.toStdString(), Color( soldermaskColor ), soldermaskImage->fileName().toStdString(), width, height );

   // make copper image
   copperImage->open();
   copperImage->close();
   renderImage( copperGerberFile.toStdString(), outlineGerberFile.toStdString(), Color( copperColor ), copperImage->fileName().toStdString(), width, height );

   // make drills image
   drillsImage->open();
   drillsImage->close();
   renderImage( drillsGerberFile.toStdString(), outlineGerberFile.toStdString(), Color( alphaMaskColor ), drillsImage->fileName().toStdString(), width, height );

   // make silkscreen image
   silkscreenImage->open();
   silkscreenImage->close();
   renderImage( silkscreenGerberFile.toStdString(), outlineGerberFile.toStdString(), Color( silkscreenColor ), silkscreenImage->fileName().toStdString(), width, height );

}


bool GerberPlotterDialog::renderImage( std::string gerberFilename, std::string outlineGerberFilename, Color layerColor, std::string imageFilename,
                                       int width, int height ) {

   mainProject = gerbv_create_project();

   //screenRenderInfo.displayWidth = width;
   //screenRenderInfo.displayHeight = height;

   //cairo_surface_t *target = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, width, height );
   //cairo_t *cr = cairo_create( target );

   Color transparent = Color( Qt::transparent );

   gerbv_open_layer_from_filename_with_color( mainProject, const_cast<gchar *>( outlineGerberFilename.c_str() ), transparent.gtk()->r(), transparent.gtk()->g(),
         transparent.gtk()->b(), transparent.gtk()->a() );
   gerbv_open_layer_from_filename_with_color( mainProject, const_cast<gchar *>( gerberFilename.c_str() ), layerColor.gtk()->r(),
         layerColor.gtk()->g(), layerColor.gtk()->b(), layerColor.gtk()->a() );

   gerbv_export_png_file_from_project_autoscaled( mainProject, width, height, const_cast<gchar *>( imageFilename.c_str() ) );

   /*
      gerbv_render_all_layers_to_cairo_target( mainProject, cr, &screenRenderInfo );

      gerbv_render_cairo_set_scale_and_translation(cr, &screenRenderInfo);

      cairo_set_source_rgba( cr, bgColor.cairo()->r(), bgColor.cairo()->g(), bgColor.cairo()->b(), 1 );
      cairo_paint( cr );

      cairo_push_group( cr );
      gerbv_render_layer_to_cairo_target( cr, mainProject->file[0], &screenRenderInfo );
      cairo_pop_group_to_source( cr );
      cairo_paint( cr );

      cairo_surface_flush( target );
   */


}
