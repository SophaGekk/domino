QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += widgets
QT += multimedia
QT += core gui multimedia
QT += multimedia multimediawidgets
SOURCES += \
    DominoLabel.cpp \
    bazaar.cpp \
    bazaaroverlay.cpp \
    bot_player.cpp \
    chatmanager.cpp \
    clickable_rect.cpp \
    gameoverdialog.cpp \
    gamewindow.cpp \
    humanplayer.cpp \
    main.cpp \
    domino_game.cpp \
    mainwindow.cpp \
    nicknamedialog.cpp \
    player.cpp \
    domino_tile.cpp \
    ruleswindow.cpp \
    settingswindow.cpp \
    statisticswindow.cpp

HEADERS += \
    bazaaroverlay.h \
    bot_player.h \
    chatmanager.h \
    clickable_rect.h \
    clickablelabel.h \
    dominolabel.h \
    gameoverdialog.h \
    gamewindow.h \
    humanplayer.h \
    mainwindow.h \
    nicknamedialog.h \
    player.h \
    domino_game.h \
    domino_tile.h \
    bazaar.h \
    ruleswindow.h \
    settingswindow.h \
    statisticswindow.h \
    test.h

FORMS += \
    BazaarOverlay.ui \
    StatisticsWindow.ui \
    gameoverdialog.ui \
    gamewindow.ui \
    mainwindow.ui \
    ruleswindow.ui \
    settingswindow.ui

RESOURCES += \
    icon.qrc \
    stats.json

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
