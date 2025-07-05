QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += widgets multimedia network
SOURCES += \
    DominoLabel.cpp \
    bazaar.cpp \
    bazaaroverlay.cpp \
    bot_player.cpp \
    chatmanager.cpp \
    clickable_rect.cpp \
    client.cpp \
    final_game_over_dialog.cpp \
    gamemodedialog.cpp \
    gameoverdialog.cpp \
    gamewindow.cpp \
    humanplayer.cpp \
    main.cpp \
    domino_game.cpp \
    mainwindow.cpp \
    networkdialog.cpp \
    nicknamedialog.cpp \
    player.cpp \
    domino_tile.cpp \
    ruleswindow.cpp \
    settingswindow.cpp \
    statisticswindow.cpp \
    waitingdialog.cpp

HEADERS += \
    bazaaroverlay.h \
    bot_player.h \
    chatmanager.h \
    clickable_rect.h \
    clickablelabel.h \
    client.h \
    dominolabel.h \
    final_game_over_dialog.h \
    gamemodedialog.h \
    gameoverdialog.h \
    gamewindow.h \
    humanplayer.h \
    mainwindow.h \
    networkdialog.h \
    nicknamedialog.h \
    player.h \
    domino_game.h \
    domino_tile.h \
    bazaar.h \
    ruleswindow.h \
    settingswindow.h \
    statisticswindow.h \
    test.h \
    waitingdialog.h

FORMS += \
    BazaarOverlay.ui \
    StatisticsWindow.ui \
    gamemodedialog.ui \
    gameoverdialog.ui \
    gamewindow.ui \
    mainwindow.ui \
    networkdialog.ui \
    ruleswindow.ui \
    settingswindow.ui \
    waitingdialog.ui

RESOURCES += \icon.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Копирование ресурсов при установке
target.path = $$PREFIX/bin
data.files = icon/*
data.path = $$PREFIX/share/domino
INSTALLS += target data
