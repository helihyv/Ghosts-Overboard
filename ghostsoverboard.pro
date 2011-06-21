#-------------------------------------------------
#
# Project created by QtCreator 2011-05-03T17:35:02
#
#-------------------------------------------------

QT       += core gui

TARGET = ghostsoverboard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    orientationcontrolledgraphicspixmapobject.cpp \
    seascene.cpp \
    seaview.cpp \
    ship.cpp \
    screenlitkeeper.cpp \
    timercontrolledgraphicspixmapobject.cpp \
    octopus.cpp

HEADERS  += mainwindow.h \
    orientationcontrolledgraphicspixmapobject.h \
    seascene.h \
    seaview.h \
    ship.h \
    screenlitkeeper.h \
    timercontrolledgraphicspixmapobject.h \
    octopus.h

CONFIG += mobility
MOBILITY = sensors
MOBILITY += systeminfo

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

maemo5 {
    icon.files = ghostsoverboard.png
    icon.path = /usr/share/icons/hicolor/64x64/apps
    INSTALLS += icon
}
