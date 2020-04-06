QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += opengl
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controllers/graphiccontroller.cpp \
    controllers/listcontroller.cpp      \
    controllers/playercontroller.cpp \
    controllers/timingcontroller.cpp    \
    controllers/volumecontroller.cpp    \
    decoder/musicfiledecoder.cpp        \
    fft/Fourier.cpp                     \
    forms/mainwindow.cpp                \
    global_types/playlist.cpp           \
    global_types/trackfile.cpp \
    graphics/graphic.cpp                \
    graphics/ogltest.cpp                \
    main.cpp \
    player_core/player.cpp


HEADERS += \
    controllers/graphiccontroller.h \
    controllers/listcontroller.h        \
    controllers/playercontroller.h \
    controllers/timingcontroller.h      \
    controllers/volumecontroller.h      \
    decoder/musicfiledecoder.h          \
    forms/mainwindow.h                  \
    global_types/musicfile.h            \
    global_types/playlist.h             \
    global_types/trackfile.h \
    graphics/graphic.h                  \
    graphics/ogltest.h                  \
    graphics/visualization.h \
    player_core/player.h \
    static_classes/fileassistant.h      \
    static_classes/styler.h \
    static_classes/util.h


FORMS += \
    forms/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32: LIBS += -L$$PWD/dep/openal/libs/Win32/ -lOpenAL32
LIBS += -lopengl32
INCLUDEPATH += $$PWD/dep/openal/include
DEPENDPATH += $$PWD/dep/openal/include

RESOURCES += \
    Icos.qrc \
    styles.qrc

DISTFILES += \
    playlists/playlist1.txt \
    playlists/run.txt
