QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    AnimItem.cpp \
    KPowerup.cpp \
    LedMeasurer.cpp \
    TopLevel.cpp \
    View.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AnimItem.h \
    Defines.h \
    KMissile.h \
    KPowerup.h \
    LedMeasurer.h \
    Sprites.h \
    TopLevel.h \
    View.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    PortedAsteroids.qrc
