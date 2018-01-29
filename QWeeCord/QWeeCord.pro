#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T20:58:38
#
#-------------------------------------------------

QT       -= gui
QT += network websockets core

CONFIG += c++14

QMAKE_CFLAGS_STATIC_LIB += -fPIC

TARGET = QWeeCord
TEMPLATE = lib

DEFINES += QWEECORD_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qweecordplugin.cpp \
    qweecord.cpp \
    qweecordguild.cpp \
    qweecordchannel.cpp \
    qweecordcategory.cpp


HEADERS += \
        qweecord.h \
        qweecord_global.h \
    qweecordguild.h \
    qweecordchannel.h \
    qweecordcategory.h

SUBDIRS += external/QWeeCord



unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QWeeChat/release/ -lQWeeChat
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QWeeChat/debug/ -lQWeeChat
else:unix: LIBS += -L$$OUT_PWD/../QWeeChat/ -lQWeeChat

INCLUDEPATH += $$PWD/../QWeeChat
DEPENDPATH += $$PWD/../QWeeChat

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QWeeChat/release/libQWeeChat.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QWeeChat/debug/libQWeeChat.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QWeeChat/release/QWeeChat.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QWeeChat/debug/QWeeChat.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../QWeeChat/libQWeeChat.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QDiscord/QDiscord/src/release/ -lQDiscord
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QDiscord/QDiscord/src/debug/ -lQDiscord
else:unix: LIBS += -L$$OUT_PWD/../QDiscord/QDiscord/src/ -lQDiscord

INCLUDEPATH += $$PWD/../QDiscord/QDiscord/src
DEPENDPATH += $$PWD/../QDiscord/QDiscord/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QDiscord/QDiscord/src/release/libQDiscord.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QDiscord/QDiscord/src/debug/libQDiscord.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QDiscord/QDiscord/src/release/QDiscord.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QDiscord/QDiscord/src/debug/QDiscord.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../QDiscord/QDiscord/src/libQDiscord.a
