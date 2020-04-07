#-------------------------------------------------
#
# Project created by QtCreator 2019-03-25T15:39:03
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GerberStencilGenerator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QT_NO_KEYWORDS

CONFIG += c++11 link_pkgconfig

SOURCES += \
    main.cpp \
    gerberstencilgenerator.cpp \
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
    settingsdialog.cpp

HEADERS += \
    color.h \
        gerberstencilgenerator.h \
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
    settingsdialog.h

FORMS += \
        gerberstencilgenerator.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

PKGCONFIG += libgerbv cairo
