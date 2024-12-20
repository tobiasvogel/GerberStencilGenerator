// workaround for bug: https://gitlab.gnome.org/GNOME/glib/-/merge_requests/1963/diffs?commit_id=552b8fd862061e74b5ad2ffe6a700f850a76f797
#include <type_traits>
// end workaround
#include "gerbv.h"
#include "gerberstencilgenerator.h"
#include "ui_gerberstencilgenerator.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QGraphicsItem>
#include <QStandardItem>
#include <QShortcut>
#include <QTemporaryFile>
#include <QScrollBar>
#include <QProcess>
#include <QMessageBox>
#include <QMovie>
#include <QAction>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
   #include <QRegularExpression>
   #include <QRegularExpressionMatch>
#endif
#include "hollowgraphicsrectitem.h"
#include "hollowroundedgraphicsrectitem.h"
#include "hollowgraphicsellipseitem.h"
#include "hollowgraphicsobrounditem.h"
#include "hollowgraphicspolygonitem.h"
#include "hollowroundedgraphicspolygonitem.h"


GerberStencilGenerator::GerberStencilGenerator( QWidget *parent ) :
   QWidget( parent ),
   ui( new Ui::GerberStencilGenerator ) {
   ui->setupUi( this );

   colorDialog = new color_widgets::ColorDialog( this );

   QCoreApplication::setOrganizationName( ORGANISATION_NAME );
   QCoreApplication::setApplicationName( APPLICATION_NAME );
   QCoreApplication::setOrganizationDomain( ORGANISATION_DOMAIN );

   this->setWindowTitle( APPLICATION_NAME );

   userSettings = new QSettings( this );

   this->restoreUserSettings();

   apertureEditScene = new QGraphicsScene( this );
   ui->apertureGraphicsView->setScene( apertureEditScene );
   gerberPreviewScene = new QGraphicsScene( this );
   ui->gerberPreviewGView->setScene( gerberPreviewScene );

   ui->gerberPreviewGView->setDragMode( QGraphicsView::ScrollHandDrag );
   ui->gerberPreviewGView->setResizeAnchor( QGraphicsView::AnchorViewCenter );

   toggleSizeAdjustControls( false );
   toggleRoundnessAjustControls( false );
   toggleInnerSizeAdjustControls( false );

   QObject::connect( ui->sizeAdjustSlider, SIGNAL( snapValueChanged( int ) ), this, SLOT( sizeSettingChanged( int ) ) );
   QObject::connect( ui->outputFileBrowseButton, SIGNAL( clicked() ), this, SLOT( outputFileBrowse() ) );
   QObject::connect( ui->inputFileBrowseButton, SIGNAL( clicked() ), this, SLOT( inputFileBrowse() ) );
   QObject::connect( ui->cornerRadiusAdjustSlider, SIGNAL( snapValueChanged( int ) ), this, SLOT( roundnessSettingChanged( int ) ) );
   QObject::connect( ui->innerSizeAdjustSlider, SIGNAL( snapValueChanged( int ) ), this, SLOT( innerSizeSettingChanged( int ) ) );
   QObject::connect( ui->apertureList, SIGNAL( currentTextChanged( QString ) ), this, SLOT( updateEditingAperture( QString ) ) );
   QObject::connect( ui->settingsButton, SIGNAL( clicked() ), this, SLOT( openSettingsDialog() ) );
   QObject::connect( ui->resetChangesButton, SIGNAL( clicked() ), this, SLOT( resetCurrentChanges() ) );
   QObject::connect( ui->acceptChangesButton, SIGNAL( clicked() ), this, SLOT( acceptChanges() ) );
   QObject::connect( ui->previewButton, SIGNAL( clicked() ), this, SLOT( generatePreview() ) );
   QObject::connect( ui->clearApertureButton, SIGNAL( clicked() ), this, SLOT( removeApertureItem() ) );
   QObject::connect( ui->quitButton, SIGNAL( clicked() ), this, SLOT( requestQuit() ) );

   QObject::connect( &thread, &RenderThread::renderedImage, this, &GerberStencilGenerator::previewUpdate );

   QObject::connect( ui->splitter, &QSplitter::splitterMoved, [this]() { resizeEvent( nullptr ); } );

   ui->previewButton->setEnabled( true );
   ui->previewButton->setToolTip( QString() );

   QAction *enableAutoPreviewAction = new QAction( ui->previewButton );
   enableAutoPreviewAction->setCheckable( true );
   enableAutoPreviewAction->setChecked( true );
   enableAutoPreviewAction->setText( tr( "Auto-Update" ) );
   ui->previewButton->addAction( enableAutoPreviewAction );
   QObject::connect( enableAutoPreviewAction, SIGNAL( toggled( bool ) ), this, SLOT( enableAutoPreview( bool ) ) );

   QAction *openPlotterDialogAction = new QAction( ui->utilitiesButton );
   openPlotterDialogAction->setCheckable( false );
   openPlotterDialogAction->setIcon( QIcon( ":/res/plot" ) );
   openPlotterDialogAction->setText( tr( "Plot Gerber Files to Image..." ) );
   QObject::connect( openPlotterDialogAction, SIGNAL( triggered( bool ) ), this, SLOT( openGerberPlotterDialog( bool ) ) );
   ui->utilitiesButton->addAction( openPlotterDialogAction );
   QAction *openCompilationDialogAction = new QAction( ui->utilitiesButton );
   openCompilationDialogAction->setCheckable( false );
   openCompilationDialogAction->setIcon( QIcon( ":/res/compile" ) );
   openCompilationDialogAction->setText( tr( "Arrange multiple Gerber Files into one File..." ) );
   QObject::connect( openCompilationDialogAction, SIGNAL( triggered( bool ) ), this, SLOT( openCompilationDialog( bool ) ) );
   ui->utilitiesButton->addAction( openCompilationDialogAction );
   QAction *openEmbedTextDialogAction = new QAction( ui->utilitiesButton );
   openEmbedTextDialogAction->setCheckable( false );
   openEmbedTextDialogAction->setIcon( QIcon( ":/res/embedtext" ) );
   openEmbedTextDialogAction->setText( tr( "Embed Text in Solder-Pads..." ) );
   QObject::connect( openEmbedTextDialogAction, SIGNAL( triggered( bool ) ), this, SLOT( openEmbedTextDialog( bool ) ) );
   ui->utilitiesButton->addAction( openEmbedTextDialogAction );

   #ifdef QT_DEBUG
   new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_D ), this, SLOT( dumpApertureList() ) );
   new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_T ), this, SLOT( dumpApertureMacro() ) );
   #endif

   aperturesWidgetList = new QStandardItemModel( ui->apertureList );

   ui->sizeAdjustSlider->setSnapping( 10, 200, 10 );
   ui->innerSizeAdjustSlider->setSnapping( -100, 100, 10 );
   ui->cornerRadiusAdjustSlider->setSnapping( 0, 100, 10 );

   loadingLabel = new QLabel( ui->gerberPreviewGView );
   loadingLabel->hide();
   loadingMovie = new QMovie( ":/res/loading", "apng" );
   loadingLabel->setMovie( loadingMovie );
   loadingLabel->setStyleSheet( "background-color: rgba(0,0,0,0.6);" );
   loadingLabel->setGeometry( QRect( 0, 0, ui->gerberPreviewGView->size().width(), ui->gerberPreviewGView->size().height() ) );
   loadingMovie->stop();

   previewZoomInButton = new QPushButton( ui->gerberPreviewGView );
   previewZoomOutButton = new QPushButton( ui->gerberPreviewGView );
   previewNormalZoomButton = new QPushButton( ui->gerberPreviewGView );

   //previewZoomInButton->setEnabled(false);
   //previewZoomOutButton->setEnabled(false);
   //previewNormalZoomButton->setEnabled(false);

   previewZoomInButton->setIcon( QIcon( ":/res/zoom-in" ) );
   previewZoomOutButton->setIcon( QIcon( ":/res/zoom-out" ) );
   previewNormalZoomButton->setIcon( QIcon( ":/res/no-zoom" ) );

   QObject::connect( previewZoomInButton, &QPushButton::clicked, [this]() { previewScaleFactor += 0.5; generatePreview(); } );
   QObject::connect( previewZoomOutButton, &QPushButton::clicked, [this]() { previewScaleFactor -= 0.5; generatePreview(); } );
   QObject::connect( previewNormalZoomButton, &QPushButton::clicked, [this]() { previewScaleFactor = 1.0; generatePreview(); } );

   if ( _showTipAtStartup ) {
      tipOfTheDayDialog = new TipOfTheDay( this );
      QObject::connect( tipOfTheDayDialog, SIGNAL( showAtStartUp( bool ) ), this, SLOT( setShowTipAtStartup( bool ) ) );
      this->showTipOfTheDay();
      bool maxtip = tipOfTheDayDialog->showTipNumber( _showTipOfTheDayNumber );

      if ( maxtip ) {
         qDebug() << "Next time request Tip Number: " << ( _showTipOfTheDayNumber + 1 );
         userSettings->setValue( "showTipOfTheDayNumber", ( _showTipOfTheDayNumber + 1 ) );

      } else {
         userSettings->setValue( "showTipOfTheDayNumber", 0 );
      }
   }

}

GerberStencilGenerator::~GerberStencilGenerator() {
   delete ui;
   overlayGerber.close();
   tempImageFile.close();
   #ifdef QT_DEBUG
   tempLogFile.close();
   #endif
}

void GerberStencilGenerator::sizeSettingChanged( int percentage ) {
   if ( ui->sizeAdjustLabel->getEnabled() ) {
      ui->sizeAdjustLabel->setText( QString( "%1%" ).arg( percentage ) );
   }

   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      QString currentAperture = aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->text();
      int apertureIndex = -1;
      int idx = 0;

      Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
         if ( QString( "%1" ).arg( findAperture.originalId ) == currentAperture.split( " " ).last() ) {
            apertureIndex = idx;
         }

         idx++;
      }

      if ( apertureIndex == -1 ) {
         throw ( "couldn't find aperture in list." );

      } else {
         flash_aperture_struct *selectedAperture = nullptr;
         selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );
         selectedAperture->resizePercentage = percentage;
         updateEditingAperture( currentAperture );
      }
   }
}

void GerberStencilGenerator::roundnessSettingChanged( int percentage ) {
   if ( ui->cornerRadiusAdjustLabel->getEnabled() ) {
      ui->cornerRadiusAdjustLabel->setText( QString( "%1%" ).arg( percentage ) );
   }

   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      QString currentAperture = aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->text();
      int apertureIndex = -1;
      int idx = 0;

      Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
         if ( QString( "%1" ).arg( findAperture.originalId ) == currentAperture.split( " " ).last() ) {
            apertureIndex = idx;
         }

         idx++;
      }

      if ( apertureIndex == -1 ) {
         throw ( "couldn't find aperture in list." );

      } else {
         flash_aperture_struct *selectedAperture = nullptr;
         selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );
         selectedAperture->roundnessPercentage = percentage;
         updateEditingAperture( currentAperture );
      }
   }
}

void GerberStencilGenerator::innerSizeSettingChanged( int percentage ) {
   if ( ui->innerSizeAdjustLabel->getEnabled() ) {
      ui->innerSizeAdjustLabel->setText( QString( "%1%" ).arg( percentage ) );
   }

   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      QString currentAperture = aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->text();
      int apertureIndex = -1;
      int idx = 0;

      Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
         if ( QString( "%1" ).arg( findAperture.originalId ) == currentAperture.split( " " ).last() ) {
            apertureIndex = idx;
         }

         idx++;
      }

      if ( apertureIndex == -1 ) {
         throw ( "couldn't find aperture in list." );

      } else {
         flash_aperture_struct *selectedAperture = nullptr;
         selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );
         selectedAperture->innerResizePercentage = percentage;
         updateEditingAperture( currentAperture );
      }
   }
}

void GerberStencilGenerator::inputFileBrowse() {
   if ( isInputFileLocked() ) {
      int ret = confirmQuestion( tr( "Do you really want to open a different File?" ), tr( "All unsafed changes will be lost!" ), QMessageBox::No );
      qDebug() << ret;

      if ( ret < 1 ) {
         return;

      } else {
         this->lockInputFile( false );
         aperturesWidgetList->clear();
         inputFileData.clear();
         apertureEditScene->clear();
         gerberPreviewScene->clear();
      }
   }

   QString fileName = QFileDialog::getOpenFileName( this, tr( "Open Gerber File" ), lastInputFileDir.path(), tr( "Gerber Files (*.*)" ) );
   ui->inputFileLineEdit->setText( fileName );
   lastInputFileDir.setPath( fileName );
   this->readGerberInputFile();
}

void GerberStencilGenerator::outputFileBrowse() {
   QString fileName = QFileDialog::getSaveFileName( this, tr( "Save File As..." ), lastOutputFileDir.path(), tr( "Gerber Files (*.*)" ) );
   ui->outputFileLineEdit->setText( fileName );
   lastOutputFileDir.setPath( fileName );
}

void GerberStencilGenerator::readGerberInputFile() {
   QFile inputFile( ui->inputFileLineEdit->text() );

   if ( !( inputFile.exists() || inputFile.isReadable() ) ) {
      return;
   }

   if ( inputFile.open( QIODevice::ReadOnly ) ) {
      QTextStream in( &inputFile );

      while ( !in.atEnd() ) {
         inputFileData.append( in.readLine() );
      }
   }

   inputFile.close();
   this->checkGerberData();
}

void GerberStencilGenerator::lockInputFile( bool lock ) {
   qDebug() << QString( "Input File " ).append( ( lock == true ) ? "Locked." : "Unlocked." );
   ui->inputFileLineEdit->setEnabled( !( lock ) );
   this->update();
}

bool GerberStencilGenerator::isInputFileLocked() {
   return ( !( ui->inputFileLineEdit->isEnabled() ) );
}

void GerberStencilGenerator::alertMessage( QString message, QString subtext = QString() ) {
   QMessageBox msgBox;
   msgBox.setText( message );

   if ( !( subtext.isEmpty() || subtext.isNull() ) ) {
      msgBox.setInformativeText( subtext );
   }

   msgBox.setStandardButtons( QMessageBox::Ok );
   msgBox.setDefaultButton( QMessageBox::Ok );
   msgBox.setIcon( QMessageBox::Information );
   msgBox.exec();
   return;
}

int GerberStencilGenerator::confirmQuestion( QString question, QString subtext = QString(), QMessageBox::StandardButton defaultButton = QMessageBox::No ) {
   QMessageBox msgBox;
   msgBox.setText( question );

   if ( !( subtext.isEmpty() || subtext.isNull() ) ) {
      msgBox.setInformativeText( subtext );
   }

   msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
   msgBox.setDefaultButton( defaultButton );
   msgBox.setIcon( QMessageBox::Question );
   int ret = msgBox.exec();

   if ( ret == QMessageBox::Yes ) {
      return 1;

   } else if ( ret == QMessageBox::No ) {
      return 0;

   } else {
      return -1;
   }
}

void GerberStencilGenerator::displayError( QString message, QString subtext = QString(), bool fatal = false ) {
   QMessageBox msgBox;
   msgBox.setText( message );

   if ( !( subtext.isEmpty() || subtext.isNull() ) ) {
      msgBox.setInformativeText( subtext );
   }

   msgBox.setStandardButtons( QMessageBox::Close );
   msgBox.setDefaultButton( QMessageBox::Close );

   if ( fatal ) {
      msgBox.setIcon( QMessageBox::Critical );

   } else {
      msgBox.setIcon( QMessageBox::Warning );
   }

   msgBox.exec();

   if ( fatal ) {
      qApp->quit();

   } else {
      return;
   }
}

void GerberStencilGenerator::checkGerberData() {
   // FIXME: Validity checks to be implemented!
   qDebug() << "validity checks omitted...";

   if ( inputFileData.isEmpty() ) {
      return;

   } else {
      this->lockInputFile( true );
   }

   this->parseGerberData();
}


void GerberStencilGenerator::updateEditingAperture( QString selectedAperture ) {
   int apertureIndex = -1;
   int idx = 0;

   Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
      if ( QString( "%1" ).arg( findAperture.originalId ) == selectedAperture.split( " " ).last() ) {
         apertureIndex = idx;
      }

      idx++;
   }

   if ( apertureIndex == -1 ) {
      throw ( "couldn't find aperture in list." );
   }

   this->markUnsavedChanges( this->checkHasUnsavedChanges( selectedAperture ) );

   apertureEditScene->clear();
   shape_type currentShape = apertureList.at( apertureIndex ).originalShape;

   if ( apertureList.at( apertureIndex ).originalShape != CIRCLE ) {
      ui->apertureGraphicsView->setBoundingBoxHeight( apertureList.at( apertureIndex ).originalHeight );
      ui->apertureGraphicsView->setBoundingBoxWidth( apertureList.at( apertureIndex ).originalWidth );

   } else {
      ui->apertureGraphicsView->setBoundingBoxHeight( apertureList.at( apertureIndex ).originalOuterDiameter );
      ui->apertureGraphicsView->setBoundingBoxWidth( apertureList.at( apertureIndex ).originalOuterDiameter );
   }

   switch ( currentShape ) {
      case CIRCLE:
         toggleSizeAdjustControls( true );
         toggleRoundnessAjustControls( false );
         toggleInnerSizeAdjustControls( apertureList.at( apertureIndex ).isHollow );

         ui->sizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).resizePercentage );

         if ( apertureList.at( apertureIndex ).isHollow ) {
            ui->innerSizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).innerResizePercentage );
         }

         drawEllipse( apertureList.at( apertureIndex ).originalOuterDiameter, apertureList.at( apertureIndex ).originalInnerDiameter,
                      apertureList.at( apertureIndex ).resizePercentage, apertureList.at( apertureIndex ).innerResizePercentage, apertureList.at( apertureIndex ).isHollow );


         break;

      case RECTANGLE:

         toggleSizeAdjustControls( true );
         toggleRoundnessAjustControls( true );
         toggleInnerSizeAdjustControls( apertureList.at( apertureIndex ).isHollow );

         ui->sizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).resizePercentage );
         ui->cornerRadiusAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).roundnessPercentage );

         if ( apertureList.at( apertureIndex ).isHollow ) {
            ui->innerSizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).innerResizePercentage );
         }

         drawRoundedRectangle( apertureList.at( apertureIndex ).originalWidth, apertureList.at( apertureIndex ).originalHeight,
                               apertureList.at( apertureIndex ).originalInnerDiameter, apertureList.at( apertureIndex ).maxCornerRadius, apertureList.at( apertureIndex ).resizePercentage,
                               apertureList.at( apertureIndex ).roundnessPercentage, apertureList.at( apertureIndex ).innerResizePercentage, apertureList.at( apertureIndex ).isHollow );


         break;

      case OBROUND:

         toggleSizeAdjustControls( true );
         toggleRoundnessAjustControls( false );
         toggleInnerSizeAdjustControls( apertureList.at( apertureIndex ).isHollow );

         ui->sizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).resizePercentage );

         if ( apertureList.at( apertureIndex ).isHollow ) {
            ui->innerSizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).innerResizePercentage );
         }

         drawObroundShape( apertureList.at( apertureIndex ).originalWidth, apertureList.at( apertureIndex ).originalHeight, apertureList.at( apertureIndex ).originalOuterDiameter,
                           apertureList.at( apertureIndex ).originalInnerDiameter, apertureList.at( apertureIndex ).resizePercentage, apertureList.at( apertureIndex ).innerResizePercentage,
                           apertureList.at( apertureIndex ).isHollow );


         break;

      case POLYGON:

         toggleSizeAdjustControls( true );
         toggleRoundnessAjustControls( true );
         toggleInnerSizeAdjustControls( apertureList.at( apertureIndex ).isHollow );
         ui->sizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).resizePercentage );
         ui->cornerRadiusAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).roundnessPercentage );

         if ( apertureList.at( apertureIndex ).isHollow ) {
            ui->innerSizeAdjustSlider->setSliderValue( apertureList.at( apertureIndex ).innerResizePercentage );
         }

         drawRoundedPolygon( apertureList.at( apertureIndex ).originalOuterDiameter, apertureList.at( apertureIndex ).originalVertices, apertureList.at( apertureIndex ).rotation,
                             apertureList.at( apertureIndex ).originalInnerDiameter, apertureList.at( apertureIndex ).resizePercentage, apertureList.at( apertureIndex ).roundnessPercentage,
                             apertureList.at( apertureIndex ).innerResizePercentage, apertureList.at( apertureIndex ).isHollow );

         break;

      default:
         qDebug() << "Error";
         throw ( "no valid shape associated with this aperture" );
   }

   if ( apertureList.at( apertureIndex ).toBeDeleted ) {
      ui->clearApertureButton->setText( tr( "Restore" ) );
      ui->clearApertureButton->setIcon( QIcon( ":/res/untrash" ) );

   } else {
      ui->clearApertureButton->setText( tr( "Delete" ) );
      ui->clearApertureButton->setIcon( QIcon( ":/res/trash" ) );
   }

}

void GerberStencilGenerator::drawRoundedRectangle( double origFlashWidth, double origFlashHeight, double origInnerDiameter, double maxCornerRadius, int resizePercentage,
      int roundnessPercentage, int innerResizePercentage, bool hollow = false ) {

   std::ignore = maxCornerRadius;

   double flashWidth = origFlashWidth / 100 * resizePercentage;
   double flashHeight = origFlashHeight / 100 * resizePercentage;
   double innerDiameter = origInnerDiameter * ( 100 + innerResizePercentage ) / 100;

   QPen emptyPen;
   emptyPen.setWidth( 0 );
   emptyPen.setStyle( Qt::SolidLine );
   emptyPen.setColor( Qt::transparent );

   HollowGraphicsRectItem *origFlash = new HollowGraphicsRectItem();
   origFlash->setRect( -( origFlashWidth / 2 ), -( origFlashHeight / 2 ), origFlashWidth, origFlashHeight );
   origFlash->setPen( emptyPen );
   origFlash->setBrush( QBrush( flashColor ) );

   if ( hollow ) {
      origFlash->setHoleSize( origInnerDiameter );
   }

   HollowGraphicsRectItem *origOverlay = new HollowGraphicsRectItem();
   origOverlay->setRect( -( origFlashWidth / 2 ), -( origFlashHeight / 2 ), origFlashWidth, origFlashHeight );
   origOverlay->setPen( emptyPen );
   origOverlay->setBrush( QBrush( QColor( flashColor.red(), flashColor.green(), flashColor.blue(), 127 ) ) );

   if ( hollow ) {
      origOverlay->setHoleSize( origInnerDiameter );
   }



   HollowRoundedGraphicsRectItem *adjustedAperture = new HollowRoundedGraphicsRectItem();
   adjustedAperture->setRect( -( flashWidth / 2 ), -( flashHeight / 2 ), flashWidth, flashHeight );
   adjustedAperture->setPen( emptyPen );
   adjustedAperture->setBrush( adjustmentColor );
   adjustedAperture->setRoundness( roundnessPercentage );

   if ( hollow ) {
      adjustedAperture->setHoleSize( innerDiameter );
   }

   apertureEditScene->addItem( origFlash );

   apertureEditScene->addItem( adjustedAperture );

   apertureEditScene->addItem( origOverlay );

   if ( origFlashWidth < flashWidth || origFlashHeight < flashHeight ) {
      origOverlay->setVisible( true );

   } else {
      origOverlay->setVisible( false );
   }

   ui->apertureGraphicsView->adjustPreviewSize( origFlashWidth, origFlashHeight );
   ui->apertureGraphicsView->update();


}

void GerberStencilGenerator::drawEllipse( double origOuterDiameter, double origInnerDiameter, int resizePercentage, int innerResizePercentage, bool hollow = false ) {
   double flashDiameter = origOuterDiameter / 100 * resizePercentage;
   double innerDiameter = origInnerDiameter * ( 100 + innerResizePercentage ) / 100;

   QPen emptyPen;
   emptyPen.setWidth( 0 );
   emptyPen.setStyle( Qt::SolidLine );
   emptyPen.setColor( Qt::transparent );

   HollowGraphicsEllipseItem *origFlash = new HollowGraphicsEllipseItem();
   origFlash->setSize( origOuterDiameter );
   origFlash->setPen( emptyPen );
   origFlash->setBrush( QBrush( flashColor ) );

   if ( hollow ) {
      origFlash->setHoleSize( origInnerDiameter );

   } else {
      origFlash->setHollow( false );
   }

   HollowGraphicsEllipseItem *origOverlay = new HollowGraphicsEllipseItem();
   origOverlay->setSize( origOuterDiameter );
   origOverlay->setPen( emptyPen );
   origOverlay->setBrush( QBrush( QColor( flashColor.red(), flashColor.green(), flashColor.blue(), 127 ) ) );

   if ( hollow ) {
      origOverlay->setHoleSize( origInnerDiameter );

   } else {
      origOverlay->setHollow( false );
   }



   HollowGraphicsEllipseItem *adjustedAperture = new HollowGraphicsEllipseItem();
   adjustedAperture->setSize( flashDiameter );
   adjustedAperture->setPen( emptyPen );
   adjustedAperture->setBrush( adjustmentColor );

   if ( hollow ) {
      adjustedAperture->setHoleSize( innerDiameter );

   } else {
      adjustedAperture->setHollow( false );
   }

   apertureEditScene->addItem( origFlash );

   apertureEditScene->addItem( adjustedAperture );

   apertureEditScene->addItem( origOverlay );

   if ( origOuterDiameter < flashDiameter ) {
      origOverlay->setVisible( true );

   } else {
      origOverlay->setVisible( false );
   }

   ui->apertureGraphicsView->adjustPreviewSize( origOuterDiameter, origOuterDiameter );
   ui->apertureGraphicsView->update();

}

void GerberStencilGenerator::drawObroundShape( double origFlashWidth, double origFlashHeight, double origOuterDiameter, double origInnerDiameter, int resizePercentage,
      int innerResizePercentage, bool hollow = false ) {
   double flashHeight = origFlashHeight / 100 * resizePercentage;
   double flashWidth = origFlashWidth / 100 * resizePercentage;
   double flashDiameter = origOuterDiameter / 100 * resizePercentage;
   double innerDiameter = origInnerDiameter * ( 100 + innerResizePercentage ) / 100;

   QPen emptyPen;
   emptyPen.setWidth( 0 );
   emptyPen.setStyle( Qt::SolidLine );
   emptyPen.setColor( Qt::transparent );

   HollowGraphicsObroundItem *origFlash = new HollowGraphicsObroundItem();
   origFlash->setWidth( origFlashWidth );
   origFlash->setHeight( origFlashHeight );
   origFlash->setPen( emptyPen );
   origFlash->setBrush( QBrush( flashColor ) );

   if ( hollow ) {
      origFlash->setHoleSize( origInnerDiameter );

   } else {
      origFlash->setHollow( false );
   }

   HollowGraphicsObroundItem *origOverlay = new HollowGraphicsObroundItem();
   origOverlay->setWidth( origFlashWidth );
   origOverlay->setHeight( origFlashHeight );
   origOverlay->setPen( emptyPen );
   origOverlay->setBrush( QBrush( QColor( flashColor.red(), flashColor.green(), flashColor.blue(), 127 ) ) );

   if ( hollow ) {
      origOverlay->setHoleSize( origInnerDiameter );

   } else {
      origOverlay->setHollow( false );
   }



   HollowGraphicsObroundItem *adjustedAperture = new HollowGraphicsObroundItem();
   adjustedAperture->setWidth( flashWidth );
   adjustedAperture->setHeight( flashHeight );
   adjustedAperture->setPen( emptyPen );
   adjustedAperture->setBrush( adjustmentColor );

   if ( hollow ) {
      adjustedAperture->setHoleSize( innerDiameter );

   } else {
      adjustedAperture->setHollow( false );
   }

   apertureEditScene->addItem( origFlash );

   apertureEditScene->addItem( adjustedAperture );

   apertureEditScene->addItem( origOverlay );

   if ( origOuterDiameter < flashDiameter ) {
      origOverlay->setVisible( true );

   } else {
      origOverlay->setVisible( false );
   }

   ui->apertureGraphicsView->adjustPreviewSize( origFlashWidth, origFlashHeight );
   ui->apertureGraphicsView->update();
}

void GerberStencilGenerator::drawRoundedPolygon( double origOuterDiameter, int origVertices, float origRotation, double origInnerDiameter, int resizePercentage,
      int roundnessPercentage, int innerResizePercentage, bool hollow = false ) {
   double flashDiameter = origOuterDiameter / 100 * resizePercentage;
   double innerDiameter = origInnerDiameter * ( 100 + innerResizePercentage ) / 100;

   QPen emptyPen;
   emptyPen.setWidth( 0 );
   emptyPen.setStyle( Qt::SolidLine );
   emptyPen.setColor( Qt::transparent );

   HollowGraphicsPolygonItem *origFlash = new HollowGraphicsPolygonItem();
   origFlash->setCircumference( origOuterDiameter );
   origFlash->setVertices( origVertices );
   origFlash->setRotation( origRotation );
   origFlash->setPen( emptyPen );
   origFlash->setBrush( QBrush( flashColor ) );

   if ( hollow ) {
      origFlash->setHoleSize( origInnerDiameter );

   } else {
      origFlash->setHollow( false );
   }

   HollowGraphicsPolygonItem *origOverlay = new HollowGraphicsPolygonItem();
   origOverlay->setCircumference( origOuterDiameter );
   origOverlay->setVertices( origVertices );
   origOverlay->setRotation( origRotation );
   origOverlay->setPen( emptyPen );
   origOverlay->setBrush( QBrush( QColor( flashColor.red(), flashColor.green(), flashColor.blue(), 127 ) ) );

   if ( hollow ) {
      origOverlay->setHoleSize( origInnerDiameter );

   } else {
      origOverlay->setHollow( false );
   }



   HollowRoundedGraphicsPolygonItem *adjustedAperture = new HollowRoundedGraphicsPolygonItem();
   adjustedAperture->setCircumference( flashDiameter );
   adjustedAperture->setVertices( origVertices );
   adjustedAperture->setRotation( origRotation );
   adjustedAperture->setRoundness( roundnessPercentage );
   adjustedAperture->setPen( emptyPen );
   adjustedAperture->setBrush( adjustmentColor );

   if ( hollow ) {
      adjustedAperture->setHoleSize( innerDiameter );

   } else {
      adjustedAperture->setHollow( false );
   }

   apertureEditScene->addItem( origFlash );

   apertureEditScene->addItem( adjustedAperture );

   apertureEditScene->addItem( origOverlay );

   if ( origOuterDiameter < flashDiameter ) {
      origOverlay->setVisible( true );

   } else {
      origOverlay->setVisible( false );
   }

   ui->apertureGraphicsView->adjustPreviewSize( origOuterDiameter, origOuterDiameter );
   ui->apertureGraphicsView->update();
}

void GerberStencilGenerator::toggleSizeAdjustControls( bool enabled = false ) {
   ui->sizeAdjustSlider->setEnabled( enabled );
   ui->sizeAdjustLabel->enable( enabled );
   ui->sizeAdjustCaption->setEnabled( enabled );
}

void GerberStencilGenerator::toggleRoundnessAjustControls( bool enabled = false ) {
   ui->cornerRadiusAdjustSlider->setEnabled( enabled );
   ui->cornerRadiusAdjustLabel->enable( enabled );
   ui->cornerRadiusAdjustCaption->setEnabled( enabled );
}

void GerberStencilGenerator::toggleInnerSizeAdjustControls( bool enabled = false ) {
   ui->innerSizeAdjustSlider->setEnabled( enabled );
   ui->innerSizeAdjustLabel->enable( enabled );
   ui->innerSizeAdjustCaption->setEnabled( enabled );
}

bool GerberStencilGenerator::checkHasUnsavedChanges( QString selectedAperture ) {
   int apertureIndex = -1;
   int idx = 0;

   Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
      if ( QString( "%1" ).arg( findAperture.originalId ) == selectedAperture.split( " " ).last() ) {
         apertureIndex = idx;
      }

      idx++;
   }

   if ( apertureIndex == -1 ) {
      throw ( "couldn't find aperture in list." );
   }

   bool changed = false;

   if ( apertureList.at( apertureIndex ).resizePercentage != apertureList.at( apertureIndex ).initialResizePercentage ) {
      changed = true;
   }

   if ( apertureList.at( apertureIndex ).roundnessPercentage != apertureList.at( apertureIndex ).initialRoundnessPercentage ) {
      changed = true;
   }

   if ( apertureList.at( apertureIndex ).innerResizePercentage != apertureList.at( apertureIndex ).initialInnerResizePercentage ) {
      changed = true;
   }

   return changed;
}

void GerberStencilGenerator::markUnsavedChanges( bool hasChanges ) {
   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      flash_aperture_struct *selectedAperture = nullptr;
      selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( ui->apertureList->currentIndex().row() ) );
      QFont itemFont;
      itemFont = aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->font();

      if ( hasChanges ) {
         itemFont.setStyle( QFont::StyleItalic );
         selectedAperture->icon.setChanged( true );

      } else {
         itemFont.setStyle( QFont::StyleNormal );
         selectedAperture->icon.setChanged( false );
      }

      aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->setFont( itemFont );
      aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->setIcon( selectedAperture->icon.getIcon() );
      this->update();
   }
}

void GerberStencilGenerator::openSettingsDialog() {
   settingsUi = new settingsDialog( this );
   //settingsUi->setModal(true);
   settingsUi->show();
}

void GerberStencilGenerator::redrawChangedColorSettings() {
   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      QString currentAperture = currentSelectedItem();
      updateEditingAperture( currentAperture );
   }

   ui->apertureGraphicsView->adjustPreviewSize();
   ui->apertureGraphicsView->update();
}

void GerberStencilGenerator::resetCurrentChanges() {
   if ( aperturesWidgetList->rowCount() == 0 ) {
      return;
   }

   QString currentAperture = currentSelectedItem();
   int apertureIndex = currentSelectedIndex();

   if ( !( this->checkHasUnsavedChanges( currentAperture ) ) ) {
      return;

   } else {
      flash_aperture_struct *selectedAperture = nullptr;
      selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );

      selectedAperture->resizePercentage = selectedAperture->initialResizePercentage;
      selectedAperture->innerResizePercentage = selectedAperture->initialInnerResizePercentage;
      selectedAperture->roundnessPercentage = selectedAperture->initialRoundnessPercentage;

      updateEditingAperture( currentAperture );
   }
}

void GerberStencilGenerator::setChangeState( change_type chg ) {
   if ( aperturesWidgetList->rowCount() == 0 ) {
      return;
   }

   QString currentAperture = currentSelectedItem();
   int apertureIndex = currentSelectedIndex();

   if ( !( this->checkHasUnsavedChanges( currentAperture ) ) ) {
      return;

   } else {
      flash_aperture_struct *selectedAperture = nullptr;
      selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );
      selectedAperture->changeState = chg;
      updateEditingAperture( currentAperture );
   }
}

void GerberStencilGenerator::setListItemBackground( int idx, QColor color ) {
   QStandardItem *item = aperturesWidgetList->item( idx );
   item->setBackground( QBrush( color, Qt::SolidPattern ) );
}

change_type GerberStencilGenerator::getChangeState( QString selectedAperture = QString() ) {
   if ( aperturesWidgetList->rowCount() == 0 ) {
      return UNCHANGED;
   }

   if ( selectedAperture.isEmpty() ) {
      selectedAperture = currentSelectedItem();
   }

   int apertureIndex = getIndexFromItemText( selectedAperture );
   return apertureList.at( apertureIndex ).changeState;
}

void GerberStencilGenerator::acceptChanges() {
   int selectedItem = ui->apertureList->currentIndex().row();
   setListItemBackground( selectedItem, lightGreen );
}

void GerberStencilGenerator::generatePreview() {
   if ( aperturesWidgetList->rowCount() < 1 ) {
      // FIXME: Show warning there is nothing loaded at this point
      return;
   }

   //loadingLabel->show();
   //loadingLabel->setGeometry(QRect(0,0,ui->gerberPreviewGView->size().width(),ui->gerberPreviewGView->size().height()));
   //loadingMovie->start();
   QSize previewSize = QSize( ui->gerberPreviewGView->contentsRect().width(), ui->gerberPreviewGView->contentsRect().height() );
   QString currentAperture = this->currentSelectedItem();
   bool ok = false;
   int apertureId = -1;

   if ( !( currentAperture.isEmpty() ) ) {
      QString intStr = currentAperture.split( " " )[currentAperture.split( " " ).count() - 1];
      apertureId = intStr.toInt( &ok );
   }

   QStringList highlightGerber;

   if ( ok ) {
      highlightGerber = generateHighlightedGerber( apertureId );
      invokeRenderer( previewSize.width()*previewScaleFactor, previewSize.height()*previewScaleFactor, highlightGerber, true );

   } else {
      qDebug() << "unable to detect aperture id";
      invokeRenderer( previewSize.width()*previewScaleFactor, previewSize.height()*previewScaleFactor, QStringList(), false );
   }
}

void GerberStencilGenerator::processStateChanged( QProcess::ProcessState state ) {
   qDebug() << "State: " << state;
}

bool GerberStencilGenerator::getAutoPreviewEnabled() {
   return autoPreview;
}

void GerberStencilGenerator::setAutoPreviewEnabled( bool option ) {
   autoPreview = option;
}

void GerberStencilGenerator::enableAutoPreview( bool enable ) {
   this->setAutoPreviewEnabled( enable );
}

void GerberStencilGenerator::previewUpdate( const QImage image, QSize size ) {
   qDebug() << "update called!";
   QImage renderImage = image;
   gerberPreviewPixmap.convertFromImage( renderImage );
   gerberPreviewScene->clear();
   gerberPreviewScene->setSceneRect( 0, 0, size.width(), size.height() );
   gerberPreviewScene->addPixmap( gerberPreviewPixmap );
   ui->gerberPreviewGView->update();
}

void GerberStencilGenerator::removeApertureItem() {
   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      flash_aperture_struct *selectedAperture = nullptr;
      int apertureIndex = currentSelectedIndex();
      selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );

      if ( selectedAperture->toBeDeleted ) {
         this->restoreApertureItem();

      } else {
         QString currentAperture = currentSelectedItem();
         int selectedItem = ui->apertureList->currentIndex().row();
         setListItemBackground( selectedItem, greyedOutColor );
         QStandardItem *item = aperturesWidgetList->item( selectedItem );
         QFont itemFont = item->font();
         itemFont.setStrikeOut( true );
         item->setFont( itemFont );
         selectedAperture->toBeDeleted = true;
         selectedAperture->icon.setDeleted( true );
         item->setIcon( selectedAperture->icon.getIcon() );
         ui->clearApertureButton->setText( tr( "Restore" ) );
         ui->clearApertureButton->setIcon( QIcon( ":/res/untrash" ) );
      }
   }
}

void GerberStencilGenerator::restoreApertureItem() {
   if ( !( aperturesWidgetList->rowCount() == 0 ) ) {
      QString currentAperture = currentSelectedItem();
      int selectedItem = ui->apertureList->currentIndex().row();
      QPalette apertureListPalette = ui->apertureList->palette();
      QColor apertureListBackground = apertureListPalette.base().color();
      setListItemBackground( selectedItem, apertureListBackground );
      QStandardItem *item = aperturesWidgetList->item( selectedItem );
      QFont itemFont = item->font();
      itemFont.setStrikeOut( false );
      item->setFont( itemFont );
      int apertureIndex = currentSelectedIndex();
      flash_aperture_struct *selectedAperture = nullptr;
      selectedAperture = const_cast<flash_aperture_struct *>( &apertureList.at( apertureIndex ) );
      selectedAperture->toBeDeleted = false;
      selectedAperture->icon.setDeleted( false );
      item->setIcon( selectedAperture->icon.getIcon() );
      ui->clearApertureButton->setText( tr( "Delete" ) );
      ui->clearApertureButton->setIcon( QIcon( ":/res/trash" ) );
   }
}

QColor GerberStencilGenerator::pickColor( QColor initialColor, QString windowTitle ) {
   colorDialog->setWindowTitle( windowTitle );
   colorDialog->setColor( initialColor );
   colorDialog->setAlphaEnabled( true );
   colorDialog->exec();
   return colorDialog->color();
}

void GerberStencilGenerator::setShowTipAtStartup( bool toggle ) {
   qDebug() << "Show Tip Of The Day at Startup:" << toggle;
   userSettings->setValue( "showAtStartup", toggle );
}

void GerberStencilGenerator::openGerberPlotterDialog( bool toggle ) {
   std::ignore = toggle;
   #ifdef QT_DEBUG
   qDebug() << "opening gerber plotter dialog";
   #endif
   plotterDialog = new GerberPlotterDialog( this );
   plotterDialog->show();
   plotterDialog->raise();
}

void GerberStencilGenerator::openCompilationDialog( bool toggle ) {
   std::ignore = toggle;
}

void GerberStencilGenerator::openEmbedTextDialog( bool toggle ) {
   std::ignore = toggle;
}

bool GerberStencilGenerator::isGerberDataLoaded() {
   return _gerberDataLoaded;
}

void GerberStencilGenerator::setGerberDataLoaded( bool loaded ) {
   _gerberDataLoaded = loaded;
}

QColor GerberStencilGenerator::getColor( QString colorId )  {
   if ( QString( "flashColor" ) == colorId ) {
      return flashColor.toRgb();

   } else if ( QString( "adjustmentColor" ) == colorId ) {
      return adjustmentColor.toRgb();

   } else if ( QString( "backgroundColor" ) == colorId ) {
      return backgroundColor.toRgb();

   } else if ( QString( "lineColor" ) == colorId ) {
      return lineColor.toRgb();

   } else {
      qDebug() << QString( "No such Color setting found: %1" ).arg( colorId );
      return QColor::Invalid;
   }
}

bool GerberStencilGenerator::setColor( QString colorId, QColor newColor ) {
   if ( QString( "flashColor" ) == colorId ) {
      flashColor = newColor;
      this->redrawChangedColorSettings();
      userSettings->setValue( "Colors/flashColor", newColor );
      return true;

   } else if ( QString( "adjustmentColor" ) == colorId ) {
      adjustmentColor = newColor;
      this->redrawChangedColorSettings();
      userSettings->setValue( "Colors/adjustmentColor", newColor );
      return true;

   } else if ( QString( "backgroundColor" ) == colorId ) {
      backgroundColor = newColor;
      this->redrawChangedColorSettings();
      userSettings->setValue( "Colors/backgroundColor", newColor );
      return true;

   } else if ( QString( "lineColor" ) == colorId ) {
      lineColor = newColor;
      this->redrawChangedColorSettings();
      userSettings->setValue( "Colors/lineColor", newColor );
      return true;

   } else {
      qDebug() << QString( "No such Color setting found: %1" ).arg( colorId );
      return false;
   }
}

QString GerberStencilGenerator::currentSelectedItem() {
   if ( aperturesWidgetList->rowCount() == 0 ) {
      return QString();
   }

   if ( ui->apertureList->currentIndex().row() < 0 ) {
      return QString();
   }

   return aperturesWidgetList->item( ui->apertureList->currentIndex().row() )->text();
}

int GerberStencilGenerator::currentSelectedIndex() {
   if ( aperturesWidgetList->rowCount() == 0 ) {
      return -1;
   }

   QString selectedAperture = currentSelectedItem();

   if ( selectedAperture.isNull() || selectedAperture.isEmpty() ) {
      return -1;
   }

   int apertureIndex = -1;
   int idx = 0;

   Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
      if ( QString( "%1" ).arg( findAperture.originalId ) == selectedAperture.split( " " ).last() ) {
         apertureIndex = idx;
      }

      idx++;
   }

   return apertureIndex;
}

int GerberStencilGenerator::getIndexFromItemText( QString itemText ) {
   if ( aperturesWidgetList->rowCount() == 0 ) {
      return -1;
   }

   if ( itemText.isEmpty() ) {
      return -1;
   }

   int apertureIndex = -1;

   for ( int i = 0; i < aperturesWidgetList->rowCount(); i++ ) {
      if ( aperturesWidgetList->item( i )->text() == itemText ) {
         apertureIndex = i;
         break;
      }

      if ( QString( "* %1" ).arg( aperturesWidgetList->item( i )->text() ) == itemText ) {
         apertureIndex = i;
         break;
      }
   }

   return apertureIndex;
}

QStandardItemModel *GerberStencilGenerator::getWidgetListModel() {
   return aperturesWidgetList;
}

QStringList GerberStencilGenerator::generateHighlightedGerber( int selectedApertureId = -1 ) {
   if ( selectedApertureId < 0 ) {
      return QStringList();
   }

   QList<int> apertureIds;

   Q_FOREACH ( flash_aperture_struct findAperture, apertureList ) {
      if ( findAperture.originalId != selectedApertureId ) {
         apertureIds.append( findAperture.originalId );
      }
   }

   QStringList tempGerberData;
   bool skipLine = false;
   #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
   QRegularExpression rx1( "^D([0-9]+)\\*$" );
   QRegularExpression rx2( "^(.*)%$" );
   QRegularExpression rx3( "^(G|M|%)(.*)$" );
   #else
   QRegExp rx1( "^D([0-9]+)\\*$" );
   QRegExp rx2( "^(.*)%$" );
   QRegExp rx3( "^(G|M|%)(.*)$" );
   #endif

   for ( int i = 0; i < inputFileData.count(); i++ ) {
      // skip empty lines
      if ( QString( inputFileData.at( i ) ).trimmed().isEmpty() ) {
         continue;
      }

      // check if new instruction begins
      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      if ( rx2.match( inputFileData.at( i ) ).hasMatch() ) {
      #else

      if ( rx2.indexIn( inputFileData.at( i ) ) > -1 ) {
      #endif
         skipLine = false;

         #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      } else if ( rx3.match( inputFileData.at( i ) ).hasMatch() ) {
         #else

      } else if ( rx3.indexIn( inputFileData.at( i ) ) > -1 ) {
         #endif
         skipLine = false;
      }

      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      if ( rx1.match( inputFileData.at( i ) ).hasMatch() ) {
      #else

      if ( rx1.indexIn( inputFileData.at( i ) ) > -1 ) {
      #endif
         skipLine = false;

         #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
         QRegularExpressionMatch match = rx1.match( inputFileData.at( i ) );

         for ( int j = 0; j < apertureIds.count(); j++ ) {
            if ( QString( match.captured( 1 ) ).toInt() == apertureIds.at( j ) ) {
         #else

         for ( int j = 0; j < apertureIds.count(); j++ ) {
            if ( QString( rx1.cap( 1 ) ).toInt() == apertureIds.at( j ) ) {
         #endif
               skipLine = true;
               break;
            }
         }
      }

      if ( !( skipLine ) ) {
         tempGerberData.append( inputFileData.at( i ) );
      }
   }

   return tempGerberData;
}

void GerberStencilGenerator::invokeRenderer( int width, int height, QStringList overlayData = QStringList(), bool useTempOverlay = false ) {

   bool _overlay = false;

   // Do we also have to include an overlay file?
   if ( useTempOverlay )  {
      if ( overlayData.isEmpty() ) {
         _overlay = false;

      } else if ( overlayData.count() < 2 ) {
         _overlay = false;

      } else {
         _overlay = true;
      }
   }

   if ( _overlay ) {
      if ( overlayGerber.isOpen() ) {
         overlayGerber.reset();
         overlayGerber.resize( 0 );

      } else {
         overlayGerber.open();
      }

      QTextStream outStream( &overlayGerber );

      Q_FOREACH ( QString outLine, overlayData ) {
         outStream << outLine.append( "\n" );
      }
   }

   thread.render( width, height, _overlay, QDir::toNativeSeparators( lastInputFileDir.absolutePath() ).toStdString(),
                  QDir::toNativeSeparators( overlayGerber.fileName() ).toStdString(), lineColor, flashColor, backgroundColor );

}

void GerberStencilGenerator::showTipOfTheDay() {
   tipOfTheDayDialog->show();
   tipOfTheDayDialog->raise();
}

void GerberStencilGenerator::requestQuit() {
   if ( !( hasUnsavedChanges ) ) {
      qApp->quit();
      #ifdef QT_DEBUG

   } else {
      qDebug() << "Unsaves changes prevent from exiting";
      // Until implemented: will just reject quitting app if hasUnsavedChanges = true..
      #endif
   }

}

void GerberStencilGenerator::resizeEvent( QResizeEvent *event ) {
   if ( event ) {
      event->accept();
   }

   qDebug()  << "resized";
   ui->apertureGraphicsView->adjustPreviewSize();
   ui->apertureGraphicsView->update();
   int previewWidgetWidth = ui->gerberPreviewGView->geometry().width();

   if ( ui->gerberPreviewGView->verticalScrollBar()->isVisible() ) {
      previewWidgetWidth -= ui->gerberPreviewGView->verticalScrollBar()->width();
   }

   int previewWidgetHeight = ui->gerberPreviewGView->geometry().height();

   if ( ui->gerberPreviewGView->horizontalScrollBar()->isVisible() ) {
      previewWidgetHeight -= ui->gerberPreviewGView->horizontalScrollBar()->height();
   }

   int previewWidgetButtonSize = 24;
   int previewWidgetButtonPadding = 4;

   previewZoomInButton->setGeometry( ( previewWidgetWidth - previewWidgetButtonSize - previewWidgetButtonPadding ),
                                     ( previewWidgetHeight - previewWidgetButtonSize - previewWidgetButtonPadding ),
                                     previewWidgetButtonSize, previewWidgetButtonSize );
   previewZoomOutButton->setGeometry( ( previewWidgetWidth - previewWidgetButtonSize - previewWidgetButtonSize - previewWidgetButtonPadding - 1 ),
                                      ( previewWidgetHeight - previewWidgetButtonSize - previewWidgetButtonPadding ),
                                      previewWidgetButtonSize, previewWidgetButtonSize );
   previewNormalZoomButton->setGeometry( ( previewWidgetWidth - previewWidgetButtonSize - previewWidgetButtonSize - previewWidgetButtonSize - previewWidgetButtonPadding -
                                           2 ),
                                         ( previewWidgetHeight - previewWidgetButtonSize - previewWidgetButtonPadding ),
                                         previewWidgetButtonSize, previewWidgetButtonSize );
   generatePreview();
}

void GerberStencilGenerator::saveUserSettings() {
   userSettings->sync();
}

void GerberStencilGenerator::restoreUserSettings() {
   QVariant color;
   QVariant string;
   QVariant boolean;
   QVariant integer;
   userSettings->beginGroup( "Colors" );
   color = userSettings->value( "flashColor", QVariant( QColor( 255, 0, 0, 255 ) ) );
   flashColor = color.value<QColor>();
   color = userSettings->value( "adjustmentColor", QVariant( QColor( 0, 255, 0, 255 ) ) );
   adjustmentColor = color.value<QColor>();
   color = userSettings->value( "backgroundColor", QVariant( QColor( 255, 255, 255, 255 ) ) );
   backgroundColor = color.value<QColor>();
   color = userSettings->value( "lineColor", QVariant( QColor( 155, 64, 255 ) ) );
   lineColor = color.value<QColor>();
   color = userSettings->value( "lightGreen", QVariant( QColor( 192, 255, 163 ) ) );
   lightGreen = color.value<QColor>();
   color = userSettings->value( "lightRed", QVariant( QColor( 255, 176, 163 ) ) );
   lightRed = color.value<QColor>();
   color = userSettings->value( "greyedOutColor", QVariant( QColor( 224, 224, 224 ) ) );
   greyedOutColor = color.value<QColor>();
   userSettings->endGroup();
   userSettings->beginGroup( "TipOfTheDay" );
   boolean = userSettings->value( "showAtStartup", QVariant( bool( true ) ) );
   _showTipAtStartup = boolean.value<bool>();
   integer = userSettings->value( "showTipOfTheDayNumber", QVariant( int( 0 ) ) );
   _showTipOfTheDayNumber = integer.value<int>();
   userSettings->endGroup();
}

void GerberStencilGenerator::parseGerberData() {
   if ( inputFileData.isEmpty() ) {
      return;
   }

   this->lockInputFile( true );
   QStringList apertureDataList;

   Q_FOREACH ( QString line, inputFileData ) {
      if ( line.length() > 4 ) {
         if ( line.mid( 0, 4 ) == "%ADD" ) {
            apertureDataList.append( line );
         }

         if ( line.mid( 0, 3 ) == "%MO" ) {
            if ( line == "%MOMM*%" ) {
               unitFormat = METRIC;

            } else if ( line == "%MOIN*%" ) {
               unitFormat = IMPERIAL;

            } else {
               throw ( "Invalid Format Mode found.\n" );
            }
         }

         if ( line.mid( 0, 6 ) == "%FSLAX" ) {
            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QRegularExpression rx( "^%FSLAX([0-9]+)Y([0-9]+)\\*%$" );

            if ( rx.match( line ).hasMatch() ) {
               if ( rx.match( line ).captured( 1 ) == rx.match( line ).captured( 2 ) ) {
                  if ( rx.match( line ).captured( 1 ).length() == 2 ) {
                     numberFormat.clear();
                     numberFormat.insert( 0, rx.match( line ).captured( 1 ).mid( 0, 1 ).toInt() );
                     numberFormat.insert( 1, rx.match( line ).captured( 1 ).mid( 1, 1 ).toInt() );
            #else
            QRegExp rx( "^%FSLAX([0-9]+)Y([0-9]+)\\*%$" );

            if ( rx.indexIn( line ) > -1 ) {
               if ( rx.cap( 1 ) == rx.cap( 2 ) ) {
                  if ( rx.cap( 1 ).length() == 2 ) {
                     numberFormat.clear();
                     numberFormat.insert( 0, rx.cap( 1 ).mid( 0, 1 ).toInt() );
                     numberFormat.insert( 1, rx.cap( 1 ).mid( 1, 1 ).toInt() );
            #endif

                  } else {
                     throw ( "Invalid number format found.\n" );
                  }

               } else {
                  throw ( "Invalid number format found.\n" );
               }

            } else {
               throw ( "Invalid number format found.\n" );
            }
         }
      }
   }

   Q_FOREACH ( QString aperture, apertureDataList ) {

      flash_aperture_struct tempAperture;

      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
      QRegularExpression rx;
      #else
      QRegExp rx;
      #endif
      rx.setPattern( "^%ADD([0-9]+)C," );

      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      if ( rx.match( aperture ).hasMatch() ) { // Circle

         tempAperture.originalId = rx.match( aperture ).captured( 1 ).toInt();
      #else

      if ( rx.indexIn( aperture ) > -1 ) { // Circle

         tempAperture.originalId = rx.cap( 1 ).toInt();
      #endif
         tempAperture.originalShape = CIRCLE;
         tempAperture.icon.setShape( CIRCLE );

         QString dimensions = aperture.right( aperture.length() - aperture.indexOf( "," ) - 1 );

         if ( dimensions.endsWith( "*%" ) ) {
            dimensions.truncate( dimensions.length() - 2 );
         }

         int segments = dimensions.count( "X" );


         if ( segments == 0 ) { // solid circle
            tempAperture.isHollow = false;
            tempAperture.originalOuterDiameter = dimensions.toDouble();
            qDebug() << dimensions;
            qDebug() << tempAperture.originalOuterDiameter;

         } else if ( segments == 1 ) { // ring shape
            tempAperture.isHollow = true;
            tempAperture.icon.setHollow( true );
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalOuterDiameter = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalInnerDiameter = rx.match( dimensions ).captured( 2 ).toDouble();
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalOuterDiameter = rx.cap( 1 ).toDouble();
               tempAperture.originalInnerDiameter = rx.cap( 2 ).toDouble();
            #endif

            } else {
               throw ( "Aperture parsing error." );
            }

         } else {
            throw ( "Aperture parsing error." );
         }

         apertureList.append( tempAperture );

         continue;
      }

      rx.setPattern( "^%ADD([0-9]+)R," );

      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      if ( rx.match( aperture ).hasMatch() ) { // Rectangle

         tempAperture.originalId = rx.match( aperture ).captured( 1 ).toInt();
      #else

      if ( rx.indexIn( aperture ) > -1 ) { // Rectangle

         tempAperture.originalId = rx.cap( 1 ).toInt();
      #endif
         tempAperture.originalShape = RECTANGLE;
         tempAperture.icon.setShape( RECTANGLE );

         QString dimensions = aperture.right( aperture.length() - aperture.indexOf( "," ) - 1 );

         if ( dimensions.endsWith( "*%" ) ) {
            dimensions.truncate( dimensions.length() - 2 );
         }


         int segments = dimensions.count( "X" );

         if ( segments == 2 ) { // contains a hole
            tempAperture.isHollow = true;
            tempAperture.icon.setHollow( true );
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalWidth = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalHeight = rx.match( dimensions ).captured( 2 ).toDouble();
               tempAperture.originalInnerDiameter = rx.match( dimensions ).captured( 3 ).toDouble();

               if ( rx.match( dimensions ).captured( 1 ) == rx.match( dimensions ).captured( 2 ) ) {
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalWidth = rx.cap( 1 ).toDouble();
               tempAperture.originalHeight = rx.cap( 2 ).toDouble();
               tempAperture.originalInnerDiameter = rx.cap( 3 ).toDouble();

               if ( rx.cap( 1 ) == rx.cap( 2 ) ) {
            #endif

                  tempAperture.originalAspectRatio = 1.0;

               } else {
                  tempAperture.originalAspectRatio = ( tempAperture.originalWidth / tempAperture.originalHeight );
               }

            } else {
               throw ( "Aperture parsing error." );
            }

         } else if ( segments == 1 ) { // regular
            tempAperture.isHollow = false;

            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalWidth = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalHeight = rx.match( dimensions ).captured( 2 ).toDouble();

               if ( rx.match( dimensions ).captured( 1 ) == rx.match( dimensions ).captured( 2 ) ) {
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalWidth = rx.cap( 1 ).toDouble();
               tempAperture.originalHeight = rx.cap( 2 ).toDouble();

               if ( rx.cap( 1 ) == rx.cap( 2 ) ) {
            #endif
                  tempAperture.originalAspectRatio = 1.0;

               } else {
                  tempAperture.originalAspectRatio = ( tempAperture.originalWidth / tempAperture.originalHeight );

               }

            } else {
               throw ( "Aperture parsing error." );
            }

         } else {
            throw ( "Aperture parsing error." );
         }

         double shorterSide = tempAperture.originalWidth;

         if ( tempAperture.originalHeight < tempAperture.originalWidth ) {
            shorterSide = tempAperture.originalHeight;
         }

         tempAperture.maxCornerRadius = shorterSide / 2;

         apertureList.append( tempAperture );


         continue;
      }

      rx.setPattern( "^%ADD([0-9]+)O," );

      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      if ( rx.match( aperture ).hasMatch() ) { // Obround

         tempAperture.originalId = rx.match( aperture ).captured( 1 ).toInt();
      #else

      if ( rx.indexIn( aperture ) > -1 ) { // Obround

         tempAperture.originalId = rx.cap( 1 ).toInt();
      #endif

         tempAperture.originalShape = OBROUND;
         tempAperture.icon.setShape( OBROUND );

         QString dimensions = aperture.right( aperture.length() - aperture.indexOf( "," ) - 1 );

         if ( dimensions.endsWith( "*%" ) ) {
            dimensions.truncate( dimensions.length() - 2 );
         }

         int segments = dimensions.count( "X" );

         if ( segments == 1 ) { // obround
            tempAperture.isHollow = false;
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalWidth = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalHeight = rx.match( dimensions ).captured( 2 ).toDouble();
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalWidth = rx.cap( 1 ).toDouble();
               tempAperture.originalHeight = rx.cap( 2 ).toDouble();
            #endif
               tempAperture.originalAspectRatio = ( tempAperture.originalWidth / tempAperture.originalHeight );

            } else {
               throw ( "Aperture parsing error." );
            }

         } else if ( segments == 2 ) { // hollow obround
            tempAperture.isHollow = true;
            tempAperture.icon.setHollow( true );
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalWidth = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalHeight = rx.match( dimensions ).captured( 2 ).toDouble();
               tempAperture.originalAspectRatio = ( tempAperture.originalWidth / tempAperture.originalHeight );
               tempAperture.originalInnerDiameter = rx.match( dimensions ).captured( 3 ).toDouble();
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalWidth = rx.cap( 1 ).toDouble();
               tempAperture.originalHeight = rx.cap( 2 ).toDouble();
               tempAperture.originalAspectRatio = ( tempAperture.originalWidth / tempAperture.originalHeight );
               tempAperture.originalInnerDiameter = rx.cap( 3 ).toDouble();
            #endif

            } else {
               throw ( "Aperture parsing error." );
            }

         } else {
            throw ( "Aperture parsing error." );
         }

         if ( tempAperture.originalWidth > tempAperture.originalHeight ) {
            tempAperture.originalOuterDiameter = tempAperture.originalHeight;

         } else if ( tempAperture.originalWidth <= tempAperture.originalHeight ) {
            tempAperture.originalOuterDiameter = tempAperture.originalWidth;
         }

         apertureList.append( tempAperture );

         continue;
      }

      rx.setPattern( "^%ADD([0-9]+)P," );

      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

      if ( rx.match( aperture ).hasMatch() ) { // Polygon

         tempAperture.originalId = rx.match( aperture ).captured( 1 ).toInt();
      #else

      if ( rx.indexIn( aperture ) > -1 ) { // Polygon

         tempAperture.originalId = rx.cap( 1 ).toInt();
      #endif
         tempAperture.originalShape = POLYGON;
         tempAperture.icon.setShape( POLYGON );

         QString dimensions = aperture.right( aperture.length() - aperture.indexOf( "," ) - 1 );

         if ( dimensions.endsWith( "*%" ) ) {
            dimensions.truncate( dimensions.length() - 2 );
         }


         int segments = dimensions.count( "X" );

         if ( segments == 1 ) { // straight n-gon
            tempAperture.isHollow = false;
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X([0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalOuterDiameter = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalVertices = rx.match( dimensions ).captured( 2 ).toInt();
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalOuterDiameter = rx.cap( 1 ).toDouble();
               tempAperture.originalVertices = rx.cap( 2 ).toInt();
            #endif

            } else {
               throw ( "Aperture parsing error." );
            }

         } else if ( segments == 2 ) { // rotated n-gon
            tempAperture.isHollow = false;
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X([0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalOuterDiameter = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalVertices = rx.match( dimensions ).captured( 2 ).toInt();
               tempAperture.rotation = rx.match( dimensions ).captured( 3 ).toFloat();
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalOuterDiameter = rx.cap( 1 ).toDouble();
               tempAperture.originalVertices = rx.cap( 2 ).toInt();
               tempAperture.rotation = rx.cap( 3 ).toFloat();
            #endif

            } else {
               throw ( "Aperture parsing error." );
            }

         } else if ( segments == 3 ) { // hollow n-gon (optionally rotated)
            tempAperture.isHollow = true;
            tempAperture.icon.setHollow( true );
            rx.setPattern( "((?:[0-9]+)?\\.[0-9]+)X([0-9]+)X((?:[0-9]+)?\\.[0-9]+)X((?:[0-9]+)?\\.[0-9]+)" );

            #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

            if ( rx.match( dimensions ).hasMatch() ) {
               tempAperture.originalOuterDiameter = rx.match( dimensions ).captured( 1 ).toDouble();
               tempAperture.originalVertices = rx.match( dimensions ).captured( 2 ).toInt();
               tempAperture.rotation = rx.match( dimensions ).captured( 3 ).toFloat();
               tempAperture.originalInnerDiameter = rx.match( dimensions ).captured( 4 ).toDouble();
            #else

            if ( rx.indexIn( dimensions ) > -1 ) {
               tempAperture.originalOuterDiameter = rx.cap( 1 ).toDouble();
               tempAperture.originalVertices = rx.cap( 2 ).toInt();
               tempAperture.rotation = rx.cap( 3 ).toFloat();
               tempAperture.originalInnerDiameter = rx.cap( 4 ).toDouble();
            #endif

            } else {
               throw ( "Aperture parsing error." );
            }

         } else {
            throw ( "Aperture parsing error." );
         }

         apertureList.append( tempAperture );
         Q_EMIT gerberDataLoaded( true );

         continue;

      } else {

         qDebug() << "Invalid Aperture found.";
         continue;
      }

   }


   Q_FOREACH ( flash_aperture_struct apt, apertureList ) {
      aperturesWidgetList->appendRow( new QStandardItem( apt.icon.getIcon(), QString( "Aperture %1" ).arg( apt.originalId ) ) );
   }

   for ( int i = 0; i < aperturesWidgetList->rowCount(); i++ ) {
      aperturesWidgetList->item( i )->setEditable( false );
   }

   ui->apertureList->setModel( aperturesWidgetList );

   if ( aperturesWidgetList->rowCount() > 0 ) {
      generatePreview();
   }

   #ifdef QT_DEBUG
   dumpApertureList();
   #endif


}


#ifdef QT_DEBUG
void  GerberStencilGenerator::dumpApertureList( void ) {
   int idx = 0;

   Q_FOREACH ( flash_aperture_struct dumpApt, apertureList ) {
      idx++;
      qDebug() << QString( "" );
      qDebug() << QString( " -- [%1]" ).arg( idx );
      qDebug() << QString( "  originalId: %1" ).arg( dumpApt.originalId );
      qDebug() << QString( "  replacementDesignator: \"" ).append( dumpApt.replacementDesignator ).append( "\"" );
      qDebug() << QString( "  originalShape: " ).append( ( dumpApt.originalShape == UNSET ) ? "Unset" :
               ( ( dumpApt.originalShape == CIRCLE ) ? "Circle" :
                 ( ( dumpApt.originalShape == RECTANGLE ) ? "Rectangle" :
                   ( ( dumpApt.originalShape == OBROUND ) ? "Obround" :
                     ( ( dumpApt.originalShape == POLYGON ) ? "Polygon" : "Macro" ) ) ) ) );
      qDebug() << QString( "  replacementShape: " ).append( ( dumpApt.replacementShape == UNSET ) ? "Unset" :
               ( ( dumpApt.replacementShape == CIRCLE ) ? "Circle" :
                 ( ( dumpApt.replacementShape == RECTANGLE ) ? "Rectangle" :
                   ( ( dumpApt.replacementShape == OBROUND ) ? "Obround" :
                     ( ( dumpApt.replacementShape == POLYGON ) ? "Polygon" : "Macro" ) ) ) ) );

      qDebug() << QString( "  isHollow: " ).append( ( dumpApt.isHollow ) ? "true" : "false" );
      qDebug() << QString( "  originalAspectRatio: %1" ).arg( dumpApt.originalAspectRatio );
      qDebug() << QString( "  originalVertices: %1" ).arg( dumpApt.originalVertices );
      qDebug() << QString( "  replacementVertices: %1" ).arg( dumpApt.replacementVertices );
      qDebug() << QString( "  originalOuterDiameter: %1" ).arg( dumpApt.originalOuterDiameter );
      qDebug() << QString( "  replacementOuterDiameter: %1" ).arg( dumpApt.replacementOuterDiameter );
      qDebug() << QString( "  originalInnerDiameter: %1" ).arg( dumpApt.originalInnerDiameter );
      qDebug() << QString( "  replacementInnerDiameter: %1" ).arg( dumpApt.replacementInnerDiameter );
      qDebug() << QString( "  originalWidth: %1" ).arg( dumpApt.originalWidth );
      qDebug() << QString( "  replacementWidth: %1" ).arg( dumpApt.replacementWidth );
      qDebug() << QString( "  originalHeight: %1" ).arg( dumpApt.originalHeight );
      qDebug() << QString( "  replacementHeight: %1" ).arg( dumpApt.replacementHeight );
      qDebug() << QString( "  rotation: %1" ).arg( static_cast<double>( dumpApt.rotation ) );
      qDebug() << QString( "  resizePercentage: %1%" ).arg( dumpApt.resizePercentage );
      qDebug() << QString( "  roundnessPercentage: %1%" ).arg( dumpApt.roundnessPercentage );
      qDebug() << QString( "  innerResizePercentage: %1%" ).arg( dumpApt.innerResizePercentage );
      qDebug() << QString( "  maxCornerRadius: %1" ).arg( dumpApt.maxCornerRadius );
      qDebug() << QString( "  initialResizePercentage: %1%" ).arg( dumpApt.initialResizePercentage );
      qDebug() << QString( "  initialRoundnessPercentage: %1%" ).arg( dumpApt.initialRoundnessPercentage );
      qDebug() << QString( "  initialInnerResizePercentage: %1%" ).arg( dumpApt.initialInnerResizePercentage );
      qDebug() << QString( "  hasUnsavedChanges: %1" ).arg( ( dumpApt.hasUnsavedChanges ) ? "true" : "false" );
      qDebug() << QString( "  toBeDeleted: %1" ).arg( ( dumpApt.toBeDeleted ) ? "true" : "false" );
      qDebug() << QString( "  changeState: " ).append( ( dumpApt.changeState == UNCHANGED ) ? "Unchanged" :
               ( ( dumpApt.changeState == DRAFT ) ? "Draft" :
                 ( ( dumpApt.changeState == ACCEPTED ) ? "Accepted" :
                   ( ( dumpApt.changeState == REVERTED ) ? "Reverted" : "Discarded" ) ) ) );

      if ( dumpApt.replacementGerber.isEmpty() ) {
         qDebug() << QString( "  replacementGerber: {}" );

      } else {
         qDebug() << QString( "  replacementGerber: { " );
         int maxlength = 0;

         Q_FOREACH ( QString str, dumpApt.replacementGerber ) {
            if ( str.length() > maxlength ) {
               maxlength = str.length();
            }
         }

         Q_FOREACH ( QString str, dumpApt.replacementGerber ) {
            int len = str.length();
            QString fill = "";

            for ( int i = 0; i < ( maxlength - len ); i++ ) {
               fill = fill.append( " " );
            }

            qDebug() << QString( "    [  " ).append( str ).append( fill ).append( "  ]" );
         }

         qDebug() << QString( "  }" );
      }

      qDebug() << QString( "========" );
   }

}

#include "enums.h"

void  GerberStencilGenerator::dumpApertureMacro( void ) {
   Q_FOREACH ( QGraphicsItem *gItem, apertureEditScene->items() ) {
      if ( gItem->type() == HOLLOWROUNDEDGRAPHICSRECTITEM ) {
         HollowRoundedGraphicsRectItem *item = nullptr;
         item = qgraphicsitem_cast<HollowRoundedGraphicsRectItem *>( gItem );
         qDebug() << item->getApertureMacro( numberFormat.at( 1 ) );

      } else if ( gItem->type() == HOLLOWROUNDEDGRAPHICSPOLYGONITEM ) {
         HollowRoundedGraphicsPolygonItem *item = nullptr;
         item = qgraphicsitem_cast<HollowRoundedGraphicsPolygonItem *>( gItem );
         qDebug() << item->getApertureMacro( numberFormat.at( 1 ) );
      }
   }
}

void GerberStencilGenerator::afterWindowIsShown() {
   this->resizeEvent( nullptr );
}
#endif
