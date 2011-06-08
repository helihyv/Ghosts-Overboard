#-------------------------------------------------
#
# Project created by QtCreator 2011-05-03T17:35:02
#
#-------------------------------------------------

QT       += core gui

TARGET = rotationcontrol2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    orientationcontrolledgraphicspixmapobject.cpp \
    timercontrolledtursas.cpp \
    seascene.cpp \
    seaview.cpp \
    ship.cpp

HEADERS  += mainwindow.h \
    orientationcontrolledgraphicspixmapobject.h \
    timercontrolledtursas.h \
    seascene.h \
    seaview.h \
    ship.h

CONFIG += mobility
MOBILITY = sensors

symbian {
    TARGET.UID3 = 0xe3f4bbc2
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}

RESOURCES += \
    orientationcontrol2pix.qrc

maemo5 {
    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications/hildon
    INSTALLS += desktopfile
}
