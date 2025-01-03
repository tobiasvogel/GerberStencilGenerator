#ifndef TIPOFTHEDAY_H
#define TIPOFTHEDAY_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QList>

#define IMAGE_RESOURCE ":/img/lightbulb"
#define IMAGE_MARGIN 12
#define TIPS_RESOURCE ":/json/tips"
#define TIPS_TRANSLATIONS ":/json/resources/translations/tips_"


enum tip_type_t { UNDEF, HTML, PLAIN };

typedef tip_type_t tip_type;

typedef struct {
   int tipId;
   tip_type tipType = UNDEF;
   QString tipTitle;
   QString tipContent;
} tip_t;

class TipOfTheDay : public QDialog {
   Q_OBJECT
public:
   TipOfTheDay( QWidget *parent = nullptr );

public Q_SLOTS:
   void setShowAtStartUp( bool toggle );
   bool showTipNumber( int showTip );
   void closeDialog( void );

protected:
   QVBoxLayout *verticalLayout;
   QHBoxLayout *horizontalTipLayout;
   QLabel *imageFrame;
   QHBoxLayout *horizontalLayout;
   QSpacerItem *imageSpacer;
   QVBoxLayout *verticalTipLayout;
   QLabel *dykLabel;
   QFrame *line;
   QLabel *tipTitle;
   QSpacerItem *titleSpacer;
   QTextBrowser *tipTextFrame;
   QHBoxLayout *horizontalButtonLayout;
   QCheckBox *showTipsCheckBox;
   QSpacerItem *verticalSpacer;
   QSpacerItem *buttonSpacer;
   QPushButton *previousButton;
   QPushButton *nextButton;
   QPushButton *closeButton;

protected Q_SLOTS:
   void loadNextTip( void );
   void loadPreviousTip( void );
   void closeEvent( QCloseEvent *event );
   bool loadTips( void );
   void showTip( void );
   void getLocale( void );

Q_SIGNALS:
   void showAtStartUp( bool );
   void showNextTipNumber( int );

private:
   bool _showAtStarup;
   int _currentTip = -1;
   bool _tipsLoaded = false;
   QString _language = "en";
   QList<tip_t> _tips;
};

#endif // TIPOFTHEDAY_H
