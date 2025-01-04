#-------------------------------------------------
#
# Project created by QtCreator 2019-03-25T15:39:03
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GerberStencilGenerator
TEMPLATE = app

exists( $$PWD/3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee ) {
    include($$PWD/3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/color_widgets.pri)
} else:exists( $$PWD/3rdparty/Qt-Color-Widgets-master ) {
    include($$PWD/3rdparty/Qt-Color-Widgets-master/color_widgets.pri)
} else {
    error("Additional \"Qt-Color-Widgets\"-library required. Please read the README-file in \"3rdparty/\" directory.")
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += sdk_no_version_check

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QT_NO_KEYWORDS

CONFIG += c++11

SOURCES += \
    src/gerberplotterdialog.cpp \
    src/main.cpp \
    src/gerberstencilgenerator.cpp \
    src/renderthread.cpp \
    src/shapeicon.cpp \
    src/slidersizelabel.cpp \
    src/apertureeditview.cpp \
    src/hollowgraphicsrectitem.cpp \
    src/hollowroundedgraphicsrectitem.cpp \
    src/hollowgraphicsellipseitem.cpp \
    src/aperturelistwidget.cpp \
    src/hollowgraphicsobrounditem.cpp \
    src/hollowgraphicspolygonitem.cpp \
    src/hollowroundedgraphicspolygonitem.cpp \
    src/settingsdialog.cpp \
    src/snapslider.cpp \
    src/tipoftheday.cpp

HEADERS += \
    src/color.h \
    src/enums.h \
    src/floatcompare.h \
    src/gerberplotterdialog.h \
    src/gerberstencilgenerator.h \
    src/renderthread.h \
    src/shapeicon.h \
    src/slidersizelabel.h \
    src/apertureeditview.h \
    src/hollowgraphicsrectitem.h \
    src/hollowroundedgraphicsrectitem.h \
    src/hollowgraphicsellipseitem.h \
    src/aperturelistwidget.h \
    src/hollowgraphicsobrounditem.h \
    src/hollowgraphicspolygonitem.h \
    src/hollowroundedgraphicspolygonitem.h \
    src/settingsdialog.h \
    src/snapslider.h \
    src/tipoftheday.h

FORMS += \
    ui/gerberplotterdialog.ui \
    ui/gerberstencilgenerator.ui \
    ui/settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

INCLUDEPATH += src/

win32 {
    RC_ICONS = resources/appicon/icon.ico
}
macx {
    ICON = resources/appicon/icon.icns
    INCLUDEPATH += $$system(pkg-config libgerbv cairo pixman-1 gtk+-2.0 --cflags | sed \'s/-I//g\')
    LIBS += $$system(pkg-config libgerbv cairo pixman-1 gtk+-2.0 --libs)
}
linux {
    ICON = resources/appicon/icon.png
    CONFIG += link_pkgconfig
    PKGCONFIG += libgerbv cairo pixman-1 gtk+-2.0
}
