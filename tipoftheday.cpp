#include "tipoftheday.h"
#include <QDebug>

TipOfTheDay::TipOfTheDay( QWidget *parent ) {
   std::ignore = parent;

   this->getLocale();

   this->setObjectName( QString::fromUtf8( "TipOfTheDayDialog" ) );
   this->resize( 533, 316 );
   this->setFixedSize( 533, 316 );
   this->setWindowTitle( tr( "Tip Of The Day" ) );
   verticalLayout = new QVBoxLayout( this );
   verticalLayout->setObjectName( QString::fromUtf8( "verticalLayout" ) );
   horizontalTipLayout = new QHBoxLayout();
   horizontalTipLayout->setObjectName( QString::fromUtf8( "horizontalTipLayout" ) );
   imageFrame = new QLabel( this );
   imageFrame->setObjectName( QString::fromUtf8( "imageFrame" ) );
   QPixmap imagePixmap( IMAGE_RESOURCE );
   imageFrame->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
   imageFrame->setMinimumWidth( 143 );
   imageFrame->setMinimumHeight( 245 );
   int frameMargin = IMAGE_MARGIN;
   int frameWidth = imageFrame->width() - 2 * frameMargin;
   int frameHeight = imageFrame->height() - 2 * frameMargin;
   imageFrame->setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
   imageFrame->setPixmap( imagePixmap.scaled( frameWidth, frameHeight, Qt::KeepAspectRatio ) );
   horizontalLayout = new QHBoxLayout( imageFrame );
   horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );

   horizontalTipLayout->addWidget( imageFrame );

   imageSpacer = new QSpacerItem( 10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );

   horizontalTipLayout->addItem( imageSpacer );

   verticalTipLayout = new QVBoxLayout();
   verticalTipLayout->setObjectName( QString::fromUtf8( "verticalTipLayout" ) );
   verticalTipLayout->setSpacing( 0 );
   dykLabel = new QLabel( this );
   dykLabel->setObjectName( QString::fromUtf8( "dykLabel" ) );
   QFont font;
   font.setPointSize( 13 );
   font.setBold( true );
   dykLabel->setFont( font );
   dykLabel->setText( tr( "Did you know..." ) );
   dykLabel->setMargin( 4 );

   verticalTipLayout->addWidget( dykLabel );

   line = new QFrame( this );
   line->setObjectName( QString::fromUtf8( "line" ) );
   line->setFrameShadow( QFrame::Plain );
   line->setFrameShape( QFrame::HLine );

   verticalTipLayout->addWidget( line );

   tipTitle = new QLabel( this );
   tipTitle->setObjectName( QString::fromUtf8( "tipTitleLabel" ) );
   QFont tfont;
   tfont.setPointSize( 11 );
   tfont.setBold( true );
   tipTitle->setFont( tfont );
   tipTitle->setMargin( 6 );

   titleSpacer = new QSpacerItem( 40, 10, QSizePolicy::Fixed, QSizePolicy::Fixed );
   verticalTipLayout->addItem( titleSpacer );

   verticalTipLayout->addWidget( tipTitle );

   tipTextFrame = new QTextBrowser( this );
   tipTextFrame->setObjectName( QString::fromUtf8( "tipTextFrame" ) );
   tipTextFrame->setAutoFillBackground( false );
   tipTextFrame->setFrameShape( QFrame::NoFrame );
   tipTextFrame->setFrameShadow( QFrame::Plain );
   tipTextFrame->setLineWidth( 0 );

   QColor background = tipTextFrame->palette().base().color();
   tipTitle->setStyleSheet( QString( "background-color: %1;" ).arg( background.name() ) );

   verticalTipLayout->addWidget( tipTextFrame );

   horizontalTipLayout->addLayout( verticalTipLayout );

   verticalLayout->addLayout( horizontalTipLayout );


   verticalSpacer = new QSpacerItem( 40, 16, QSizePolicy::Fixed, QSizePolicy::Fixed );
   verticalLayout->addItem( verticalSpacer );

   horizontalButtonLayout = new QHBoxLayout();
   horizontalButtonLayout->setObjectName( QString::fromUtf8( "horizontalButtonLayout" ) );
   showTipsCheckBox = new QCheckBox( this );
   showTipsCheckBox->setObjectName( QString::fromUtf8( "showTipsCheckBox" ) );
   showTipsCheckBox->setText( tr( "Show tips at startup" ) );
   showTipsCheckBox->setChecked( true );

   horizontalButtonLayout->addWidget( showTipsCheckBox );

   buttonSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

   horizontalButtonLayout->addItem( buttonSpacer );

   previousButton = new QPushButton( this );
   previousButton->setObjectName( QString::fromUtf8( "previousButton" ) );
   previousButton->setText( tr( "%1 &Previous" ).arg( "\u00AB" ) );
   previousButton->setEnabled( false );

   horizontalButtonLayout->addWidget( previousButton );

   nextButton = new QPushButton( this );
   nextButton->setObjectName( QString::fromUtf8( "nextButton" ) );
   nextButton->setText( tr( "&Next %1" ).arg( "\u00BB" ) );

   horizontalButtonLayout->addWidget( nextButton );

   closeButton = new QPushButton( this );
   closeButton->setObjectName( QString::fromUtf8( "closeButton" ) );
   closeButton->setText( tr( "&Close" ) );

   horizontalButtonLayout->addWidget( closeButton );

   verticalLayout->addLayout( horizontalButtonLayout );

   _showAtStarup = showTipsCheckBox->isChecked();

   QObject::connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeDialog() ) );
   QObject::connect( nextButton, SIGNAL( clicked() ), this, SLOT( loadNextTip() ) );
   QObject::connect( previousButton, SIGNAL( clicked() ), this, SLOT( loadPreviousTip() ) );

   bool ok;
   ok = this->loadTips();

   if ( ok ) {
      _currentTip = 0;
      this->showTip();
   }
}

void TipOfTheDay::setShowAtStartUp( bool toggle ) {
   _showAtStarup = toggle;
   Q_EMIT showAtStartUp( toggle );
}

bool TipOfTheDay::showTipNumber( int showTip = 0 ) {
   qDebug() << "Tip Number requested: " << showTip;

   if ( showTip > 0 ) {
      _currentTip = showTip - 1;

   } else {
      _currentTip = 0;
   }

   this->showTip();

   if ( _currentTip < ( showTip - 1 ) ) {
      return false;

   } else {
      return true;
   }
}

void TipOfTheDay::closeDialog() {
   this->close();
}

void TipOfTheDay::loadNextTip( void ) {
   int nextTip = _currentTip + 1;

   if ( ( _tips.count() - 1 ) >= nextTip ) {
      _currentTip = nextTip;
   }

   this->showTip();
}

void TipOfTheDay::loadPreviousTip( void ) {
   int previousTip = _currentTip - 1;

   if ( previousTip >= 0 ) {
      _currentTip = previousTip;
   }

   this->showTip();

}

void TipOfTheDay::closeEvent( QCloseEvent *event ) {
   if ( _showAtStarup != showTipsCheckBox->isChecked() ) {
      Q_EMIT showAtStartUp( showTipsCheckBox->isChecked() );
   }

   event->accept();
}

bool TipOfTheDay::loadTips() {
   QFile file;

   if ( _language != "en" ) {
      file.setFileName( QString( TIPS_TRANSLATIONS ).append( _language ).append( ".json" ) );
      qDebug() << "Trying to load translations from " << file.fileName();
   }

   if ( !file.exists() ) { // Fallback if there is no such translation
      file.setFileName( TIPS_RESOURCE );
   }

   file.open( QIODevice::ReadOnly | QIODevice::Text );
   QString val = file.readAll();
   file.close();
   QJsonDocument d = QJsonDocument::fromJson( val.toUtf8() );
   QJsonObject jObjs = d.object();
   QJsonValue jTips = jObjs.value( QString( "tips" ) );
   QJsonArray jArray = jTips.toArray();

   Q_FOREACH ( QJsonValue jItem, jArray ) {
      QJsonObject tip = jItem["tip"].toObject();
      tip_t tipItem;
      tipItem.tipId = _tips.count() + 1;
      tipItem.tipTitle = tip.value( QString( "title" ) ).toString();

      if ( tip.keys().contains( "html" ) ) {
         tipItem.tipType = HTML;
         tipItem.tipContent = tip.value( "html" ).toString();

      } else if ( tip.keys().contains( "plain" ) ) {
         tipItem.tipType = PLAIN;
         tipItem.tipContent = tip.value( "plain" ).toString();
      }

      if ( !( tipItem.tipType == UNDEF ) ) {
         _tips.append( tipItem );
      }
   }

   if ( _tips.count() > 0 ) {
      _tipsLoaded = true;
      return true;
   } {

      return false;
   }

   return false;
}

void TipOfTheDay::showTip() {
   if ( !_tipsLoaded ) {
      return;
   }

   if ( _currentTip < 0 || _currentTip > ( _tips.count() - 1 ) ) {
      _currentTip = 0;
   }

   tipTitle->setText( _tips.at( _currentTip ).tipTitle );

   if ( _tips.at( _currentTip ).tipType == PLAIN ) {
      tipTextFrame->setText( _tips.at( _currentTip ).tipContent );

   } else if ( _tips.at( _currentTip ).tipType == HTML ) {
      tipTextFrame->setHtml( _tips.at( _currentTip ).tipContent );

   } else {
      tipTextFrame->setText( tr( "Error loading Tip" ) );
   }

   if ( _currentTip >= ( _tips.count() - 1 ) ) {
      nextButton->setEnabled( false );

   } else {
      nextButton->setEnabled( true );
   }

   if ( _currentTip <= 0 ) {
      previousButton->setEnabled( false );

   } else {
      previousButton->setEnabled( true );
   }
}

void TipOfTheDay::getLocale() {
   QString locale = QLocale::system().name().split( "_" ).at( 0 );
   _language = locale.toLower();
}
