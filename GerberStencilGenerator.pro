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
    gerberplotterdialog.cpp \
    main.cpp \
    gerberstencilgenerator.cpp \
    renderthread.cpp \
    shapeicon.cpp \
    slidersizelabel.cpp \
    apertureeditview.cpp \
    hollowgraphicsrectitem.cpp \
    hollowroundedgraphicsrectitem.cpp \
    hollowgraphicsellipseitem.cpp \
    aperturelistwidget.cpp \
    hollowgraphicsobrounditem.cpp \
    hollowgraphicspolygonitem.cpp \
    hollowroundedgraphicspolygonitem.cpp \
    settingsdialog.cpp \
    snapslider.cpp \
    tipoftheday.cpp

HEADERS += \
    color.h \
    enums.h \
    floatcompare.h \
    gerberplotterdialog.h \
    gerberstencilgenerator.h \
    renderthread.h \
    shapeicon.h \
    slidersizelabel.h \
    apertureeditview.h \
    hollowgraphicsrectitem.h \
    hollowroundedgraphicsrectitem.h \
    hollowgraphicsellipseitem.h \
    aperturelistwidget.h \
    hollowgraphicsobrounditem.h \
    hollowgraphicspolygonitem.h \
    hollowroundedgraphicspolygonitem.h \
    settingsdialog.h \
    snapslider.h \
    tipoftheday.h

FORMS += \
    gerberplotterdialog.ui \
    gerberstencilgenerator.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

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
    PKGCONFIG += libgerbv cairo pixman-1
}
