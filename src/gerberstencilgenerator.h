﻿#ifndef GERBERSTENCILGENERATOR_H
#define GERBERSTENCILGENERATOR_H

#include "enums.h"
#include <QWidget>
#include <QDir>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QProcess>
#include <QTemporaryFile>
#include <QSettings>
#include <QSplitter>
#include "apertureeditview.h"
#include "slidersizelabel.h"
#include "aperturelistwidget.h"
#include "settingsdialog.h"
#include "gerberplotterdialog.h"
#include "color.h"
#include "shapeicon.h"
#include "tipoftheday.h"
#include "snapslider.h"
#include "renderthread.h"
#include "QtColorWidgets/color_dialog.hpp"

#define APPLICATION_NAME "Gerber Stencil Generator"
#define APPLICATION_AUTHOR "Tobias X Vogel"
#define ORGANISATION_NAME "Tobias Vogel"
#define ORGANISATION_DOMAIN "tobiasvogel.tv"


typedef struct flash_aperture_struct {
   int originalId = 0;
   QString replacementDesignator;
   shape_type originalShape = UNSET;
   shape_type replacementShape = UNSET;
   bool isHollow = false;
   double originalAspectRatio = 1.0;
   int originalVertices = 1;
   int replacementVertices = 0;
   double originalOuterDiameter = 0.0;
   double replacementOuterDiameter = 0.0;
   double originalInnerDiameter = 0.0;
   double replacementInnerDiameter = 0.0;
   double originalWidth = 0.0;
   double replacementWidth = 0.0;
   double originalHeight = 0.0;
   double replacementHeight = 0.0;
   double maxCornerRadius = 0.0;
   float rotation = 0.0;
   int resizePercentage = 100;
   int initialResizePercentage = 100;
   int roundnessPercentage = 0;
   int initialRoundnessPercentage = 0;
   signed int innerResizePercentage = 0;
   signed int initialInnerResizePercentage = 0;
   QStringList replacementGerber;
   bool hasUnsavedChanges = false;
   bool toBeDeleted = false;
   change_type changeState = UNCHANGED;
   ShapeIcon icon;
} flash_aperture_struct;

namespace Ui {
class GerberStencilGenerator;
class settingsDialog;
class sliderSizeLabel;
class snapSlider;
class apertureListWidget;
}

class GerberStencilGenerator : public QWidget {
   Q_OBJECT

public:
   explicit GerberStencilGenerator( QWidget *parent = nullptr );
   ~GerberStencilGenerator();

   QColor getColor( QString colorId = "" );
   bool setColor( QString colorId = "", QColor newColor = Qt::transparent );

   Q_ENUM( unit_type );
   Q_ENUM( change_type );
   Q_ENUM( shape_type );

private:
   Ui::GerberStencilGenerator *ui;
   settingsDialog *settingsUi;
   TipOfTheDay *tipOfTheDayDialog;
   GerberPlotterDialog *plotterDialog;
   QSettings *userSettings;
   QString gerbvApplicationPath;
   QDir lastInputFileDir = QDir::homePath();
   bool inputFileLock = false;
   QDir lastOutputFileDir = QDir::homePath();
   QStringList inputFileData;
   unit_type unitFormat = UNKNOWN;
   QList<int> numberFormat;
   QList<flash_aperture_struct> apertureList;
   QGraphicsScene *apertureEditScene;
   QGraphicsScene *gerberPreviewScene;
   QPainter *previewPainter;
   QColor flashColor;
   QColor adjustmentColor;
   QColor backgroundColor;
   QColor lineColor;
   QColor lightGreen;
   QColor lightRed;
   QColor greyedOutColor;
   QStandardItemModel *aperturesWidgetList;
   QPixmap gerberPreviewPixmap;
   bool gerbv_confirmed = false;
   bool autoPreview = true;
   bool hasUnsavedChanges = false;
   QLabel *loadingLabel;
   QMovie *loadingMovie;
   QStringList processArguments;
   QTemporaryFile overlayGerber;
   #ifdef QT_DEBUG
   QTemporaryFile tempLogFile;
   #endif
   QTemporaryFile tempImageFile;
   bool _showTipAtStartup;
   int _showTipOfTheDayNumber;

   QPushButton *previewZoomInButton;
   QPushButton *previewZoomOutButton;
   QPushButton *previewNormalZoomButton;

   double previewScaleFactor = 1.00;

   color_widgets::ColorDialog *colorDialog;

   RenderThread thread;

   bool _gerberDataLoaded = false;

protected:
   void resizeEvent( QResizeEvent *event );
   void showEvent( QShowEvent *event ) {
      QWidget::showEvent( event );
      QMetaObject::invokeMethod( this, "afterWindowIsShown", Qt::ConnectionType::QueuedConnection );
   }

   void saveUserSettings( void );
   void restoreUserSettings( void );

   QString currentSelectedItem( void );
   int currentSelectedIndex( void );
   int getIndexFromItemText( QString itemText );
   QStandardItemModel *getWidgetListModel( void );
   QStringList generateHighlightedGerber( int selectedApertureId );
   void invokeRenderer( int width, int height, QStringList overlayData, bool useTempOverlay );
   void showTipOfTheDay( void );

Q_SIGNALS:
   void gerberDataLoaded( bool loaded );

protected Q_SLOTS:
   void sizeSettingChanged( int percentage );
   void roundnessSettingChanged( int percentage );
   void innerSizeSettingChanged( int percentage );
   void inputFileBrowse();
   void outputFileBrowse();
   void readGerberInputFile();
   void lockInputFile( bool lock );
   bool isInputFileLocked( void );
   void alertMessage( QString message, QString subtext );
   int confirmQuestion( QString question, QString subtext, QMessageBox::StandardButton defaultButton );
   void displayError( QString message, QString subtext, bool fatal );
   void checkGerberData();
   void parseGerberData();
   void updateEditingAperture( QString selectedAperture );
   void drawRoundedRectangle( double origFlashWidth, double origFlashHeight, double origInnerDiameter, double maxCornerRadius, int resizePercentage, int roundnessPercentage,
                              int innerResizePercentage, bool hollow );
   void drawEllipse( double origOuterDiameter, double origInnerDiameter, int resizePercentage, int innerResizePercentage, bool hollow );
   void drawObroundShape( double origFlashWidth, double origFlashHeight, double origOuterDiameter, double origInnerDiameter, int resizePercentage, int innerResizePercentage,
                          bool hollow );
   void drawRoundedPolygon( double origOuterDiameter, int origVertices, float origRotation, double origInnerDiameter, int resizePercentage, int roundnessPercentage,
                            int innerResizePercentage, bool hollow );
   void toggleSizeAdjustControls( bool enabled );
   void toggleRoundnessAjustControls( bool enabled );
   void toggleInnerSizeAdjustControls( bool enabled );
   bool checkHasUnsavedChanges( QString selectedAperture );
   void markUnsavedChanges( bool hasChanges = true );
   void openSettingsDialog( void );
   void redrawChangedColorSettings( void );
   void resetCurrentChanges();
   void setChangeState( change_type chg = UNCHANGED );
   void setListItemBackground( int idx, QColor color );
   change_type getChangeState( QString selectedAperture );
   void acceptChanges( void );
   void generatePreview( void );
   void processStateChanged( QProcess::ProcessState );
   bool getAutoPreviewEnabled( void );
   void setAutoPreviewEnabled( bool option );
   void enableAutoPreview( bool enable );
   void previewUpdate( const QImage image, QSize size );
   void removeApertureItem( void );
   void restoreApertureItem( void );
   QColor pickColor( QColor initialColor, QString windowTitle );
   void setShowTipAtStartup( bool toggle );
   void setShowNextTipNumber( int tipNumber );
   void openGerberPlotterDialog( void );
   void openCompilationDialog( void );
   void openEmbedTextDialog( void );
   bool isGerberDataLoaded( void );
   void setGerberDataLoaded( bool loaded );
   void requestQuit( void );
   #ifdef QT_DEBUG
   void dumpApertureList();
   void dumpApertureMacro();
   #endif

private Q_SLOTS:
   void afterWindowIsShown( void );

   friend class sliderSizeLabel;
   friend class snapSlider;
   friend class apertureEditView;
   friend class settingsDialog;
   friend class apertureListWidget;
};

#endif // GERBERSTENCILGENERATOR_H
