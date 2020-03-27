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
    fileassistant.cpp \
    graphic.cpp \
    listcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    musicfiledecoder.cpp \
    ogltest.cpp \
    player.cpp \
    playlist.cpp \
    styler.cpp \
    timingcontroller.cpp \
    volumecontroller.cpp

HEADERS += \
    Visualization.h \
    fileassistant.h \
    graphic.h \
    listcontroller.h \
    mainwindow.h \
    musicfile.h \
    musicfiledecoder.h \
    ogltest.h \
    player.h \
    playlist.h \
    styler.h \
    timingcontroller.h \
    util.h \
    volumecontroller.h

FORMS += \
    form.ui \
    mainwindow.ui


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
