#-------------------------------------------------
#
# Project created by QtCreator 2011-05-03T17:35:02
#
#-------------------------------------------------

QT       += core gui
QT      += dbus

TARGET = ghostsoverboard
TEMPLATE = app

VERSION = 0.4.0

SOURCES += main.cpp\
    orientationcontrolledgraphicspixmapobject.cpp \
    seascene.cpp \
    ship.cpp \
    screenlitkeeper.cpp \
    timercontrolledgraphicspixmapobject.cpp \
    octopus.cpp \
    level.cpp \
    seaview.cpp \
    levelset.cpp

HEADERS  += \
    orientationcontrolledgraphicspixmapobject.h \
    seascene.h \
    ship.h \
    screenlitkeeper.h \
    timercontrolledgraphicspixmapobject.h \
    octopus.h \
    level.h \
    seaview.h \
    levelset.h

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

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

unix:!symbian:!maemo5 {
    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
}


