QT = core
QT += widgets
QT += multimedia
QT += core gui multimedia
QT += core network
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        server.cpp
HEADERS += \
    server.h

INCLUDEPATH += ../domino
SOURCES += ../domino/domino_game.cpp \
           ../domino/bazaar.cpp \
           ../domino/player.cpp \
           ../domino/humanplayer.cpp \
           ../domino/bot_player.cpp \
           ../domino/domino_tile.cpp

HEADERS += ../domino/domino_game.h \
           ../domino/bazaar.h \
           ../domino/player.h \
           ../domino/humanplayer.h \
           ../domino/bot_player.h \
           ../domino/domino_tile.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


