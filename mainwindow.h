// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "gamewindow.h" // Игровое окно
#include "settingswindow.h"  // Окно настроек
#include "ruleswindow.h"  // Окно правил
#include "statisticswindow.h" // Окно статистки
#include <QSoundEffect>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // Исправлено: MainWindow вместо DominoWindow
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool highlightEnabled() const;
    void startNewGame();
    bool loadSavedGame();

private slots:
    void saveCurrentSettings();
    void onPlayClicked();
    void onRulesClicked();
    void onStatsClicked();
    void showSettingsWindow();
    void applySettings(int players, int bots, bool showReserve, bool soundEnabled, bool highlight);

private:
    Ui::MainWindow *ui; // Исправлено: MainWindow вместо DominoWindow
    GameWindow *gameWindow = nullptr;
    SettingsWindow *settingsWindow = nullptr;
    StatisticsWindow *statsWindow = nullptr;
    RulesWindow *ruleswindow = nullptr;
    QSoundEffect *soundEffect;
    QSoundEffect* tileSelectSound;
    QSoundEffect* tilePlaceSound;
    QMediaPlayer* backgroundMusic;
    QAudioOutput* audioOutput;
    bool isHighlightEnabled = false;
    bool checkCurrentGame();

    int currentPlayers;
    int currentBots;
};
#endif // MAINWINDOW_H
