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
#include "waitingdialog.h"
#include "gamemodedialog.h"
#include "networkdialog.h"

enum class SettingType {
    Critical,    // Изменение игроков/ботов
    NonCritical  // Настройки интерфейса/звука
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool highlightEnabled() const;
    void applyNonCriticalSettings();
    bool checkCurrentGame(SettingType settingType);
    void startHostGame(const QString& playerName);

private slots:
    void saveCurrentSettings();
    void onPlayClicked();
    void onRulesClicked();
    void onStatsClicked();
    void showSettingsWindow();
    void applySettings(int players, int bots, bool showReserve, bool soundEnabled, bool highlight);
    void onSessionCreated(const QString& sessionCode);
    void onPlayerJoined(const QString& playerName);
    void onJoined(int currentPlayers, int requiredPlayers);
    void onGameStarted(const QJsonObject& state);
    void onNetworkError(const QString& message);

private:
    Ui::MainWindow *ui;
    GameWindow *gameWindow = nullptr;
    Client *сlient = nullptr;
    WaitingDialog *waitingDialog = nullptr;
    SettingsWindow *settingsWindow = nullptr;
    StatisticsWindow *statsWindow = nullptr;
    RulesWindow *ruleswindow = nullptr;

    QMediaPlayer* backgroundMusic;
    QAudioOutput* audioOutput;
    bool isHighlightEnabled = false;

    int currentPlayers;
    int currentBots;
    QStringList names;

    void startLocalGame();
    void showNetworkDialog();
    void startHostGame(const QString& playerName, const QString& host, quint16 port, int playersCount);
    void joinSession(const QString& playerName, const QString& host, quint16 port, const QString& sessionCode);
    void showWaitingDialog();
};
#endif // MAINWINDOW_H
