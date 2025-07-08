#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "domino_game.h"
#include "player.h"
#include <QMessageBox>
#include <QTime>
#include "statisticswindow.h"
#include <QFileDialog>
#include <QShortcut>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "nicknamedialog.h"
#include <QDir>
#include <QStandardPaths>
#include <QJsonArray>
#include "gamewindow.h"
#include "client.h"
#include "networkdialog.h"
#include "waitingdialog.h"

MainWindow ::MainWindow (QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Настройка главного окна
    setWindowTitle(tr("Игра Домино"));
    ui->setupUi(this);
    // Настройки окна
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowFullscreenButtonHint);
    setFixedSize(size()); // Фиксирует размер после инициализации интерфейса

    // Фоновая музыка
    audioOutput = new QAudioOutput(this);
    backgroundMusic = new QMediaPlayer(this);
    backgroundMusic->setAudioOutput(audioOutput);
    backgroundMusic->setSource(QUrl::fromLocalFile("audio/1tema.mp3"));
    audioOutput->setVolume(0.1);

    // Зацикливание музыки
    connect(backgroundMusic, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            backgroundMusic->setPosition(0); // Перемотка в начало
            backgroundMusic->play();         // Запуск с начала
        }
    });


    QSettings settings("MyCompany", "DominoGame");
    currentPlayers = settings.value("players", 2).toInt();
    currentBots = settings.value("bots", 0).toInt();
    bool isSound = settings.value("soundEnabled", true).toBool();
    // Настройка соединений
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(ui->rulesButton, &QPushButton::clicked, this, &MainWindow::onRulesClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::showSettingsWindow);
    connect(ui->statsButton, &QPushButton::clicked, this, &MainWindow::onStatsClicked);

    connect(backgroundMusic, &QMediaPlayer::mediaStatusChanged, this, [this, isSound](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia && isSound) {
            backgroundMusic->play();
        }
    });

    QShortcut* saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::saveCurrentSettings);
}

void MainWindow::onPlayClicked() {
    GameModeDialog modeDialog(this);
    if (modeDialog.exec() != QDialog::Accepted) {
        return; // Пользователь отменил
    }

    if (modeDialog.isNetworkGameSelected()) {
        showNetworkDialog();
    } else {
        startLocalGame();
    }
}

void MainWindow::startLocalGame()
{
    QSettings settings("MyCompany", "DominoGame");
    int players = settings.value("players", 2).toInt();
    int bots = settings.value("bots", 0).toInt();
    bool showReserve = settings.value("showReserve", true).toBool();

    // Если игра уже запущена и не завершена - просто показываем окно
    if (gameWindow && !gameWindow->isGameOver()) {
        gameWindow->show();
        this->hide();
        return;
    }

    // Если игра завершена или не существует - создаем новую
    int humanPlayers = players - bots;
    NicknameDialog dialog(humanPlayers, this);

    if (dialog.exec() == QDialog::Accepted) {
        if (gameWindow) {
            delete gameWindow;
            gameWindow = nullptr;
        }

        names = dialog.getNames();
        existingGame = nullptr;
        gameWindow = new GameWindow(players, bots, showReserve, names, nullptr);
        if (players > 1 && bots == 0) {
            client = new Client();

            // Настраиваем связи
            client->setGameWindow(gameWindow);
            gameWindow->setClient(client);

            // Подключение сигналов
            connect(gameWindow, &GameWindow::requestBazaar, client, &Client::sendBazaarRequest);
            connect(gameWindow, &GameWindow::requestSkip, client, &Client::sendSkipRequest);
            connect(gameWindow, &GameWindow::sendNetworkChat, client, &Client::sendChatMessage);

            // Подключение к серверу
            client->connectToServer("127.0.0.1", 2323, "Игрок11");
        }
        else {
            gameWindow->setClient(nullptr);
        }


        gameWindow->setHighlightEnabled(settings.value("highlight", false).toBool());
        connect(gameWindow, &GameWindow::returnToMainMenu, this, &MainWindow::show);

        if (settings.value("soundEnabled", true).toBool()) {
            backgroundMusic->play();
        }
        else
        {
            backgroundMusic->stop();
        }

        gameWindow->show();
        this->hide();
    }
}

void MainWindow::showNetworkDialog()
{
    NetworkDialog networkDialog(this);
    if (networkDialog.exec() == QDialog::Accepted) {
        QString playerName = networkDialog.getPlayerName();
        QString host = networkDialog.getHostAddress();
        quint16 port = networkDialog.getPort();

        if (networkDialog.isHosting()) {
            int playersCount = networkDialog.getPlayersCount();
            startHostGame(playerName, host, port, playersCount);
        } else {
            QString sessionCode = networkDialog.getSessionCode();
            joinSession(playerName, host, port, sessionCode);
        }
    }
}

void MainWindow::startHostGame(const QString& playerName, const QString& host, quint16 port, int playersCount)
{
    if (client) {
        delete client;
    }
    client = new Client(this);

    connect(client, &Client::sessionCreated, this, &MainWindow::onSessionCreated);
    connect(client, &Client::playerJoined, this, &MainWindow::onPlayerJoined);
    connect(client, &Client::gameStarted, this, &MainWindow::onGameStarted);
    connect(client, &Client::errorOccurred, this, &MainWindow::onNetworkError);
    connect(client, &Client::sessionUpdated, this, &MainWindow::onSessionUpdated);

    client->connectToServer(host, port, playerName);
    client->createSession(playerName, playersCount);

    showWaitingDialog();
}

void MainWindow::joinSession(const QString& playerName, const QString& host, quint16 port, const QString& sessionCode)
{
    if (client) {
        delete client;
    }
    client = new Client(this);

    connect(client, &Client::playerJoined, this, &MainWindow::onPlayerJoined);
    connect(client, &Client::gameStarted, this, &MainWindow::onGameStarted);
    connect(client, &Client::errorOccurred, this, &MainWindow::onNetworkError);
    connect(client, &Client::sessionUpdated, this, &MainWindow::onSessionUpdated);

    client->connectToServer(host, port, playerName);
    client->joinSession(sessionCode, playerName);

    showWaitingDialog();
}

void MainWindow::onSessionUpdated(int players, int required)
{
    if (waitingDialog) {
        waitingDialog->setPlayersCount(players, required);
        waitingDialog->setSessionCode(sessionCode);
    }
}

void MainWindow::onSessionCreated(const QString& sessionCodes, const QString& playerName, int players, int required)
{
    if (waitingDialog) {
        waitingDialog->setSessionCode(sessionCodes);
        waitingDialog->setPlayersCount(players, required);
        waitingDialog->addPlayer(playerName);
        sessionCode = sessionCodes;
    }
    QMessageBox::information(this, "Сессия создана", "Код вашей сессии: " + sessionCodes + "\n\n" "Сообщите этот код другим игрокам для подключения.");
}


void MainWindow::onPlayerJoined(const QString& playerName, int currentPlayers, int requiredPlayers)
{
    if (waitingDialog) {
        waitingDialog->addPlayer(playerName);
        waitingDialog->setPlayersCount(currentPlayers, requiredPlayers);
    }
}

void MainWindow::onGameStarted(const QJsonObject& state)
{
    if (waitingDialog) {
        waitingDialog->close();
        delete waitingDialog;
        waitingDialog = nullptr;
    }

    // Создаем пустую игру для десериализации
    DominoGame* networkGame = new DominoGame(0, 0, QStringList(), this);
    networkGame->deserializeFromJson(state);
    existingGame = networkGame;

    // Получаем список имен игроков из состояния
    QStringList playerNames;
    QJsonArray playersArray = state["players"].toArray();
    for (const QJsonValue& playerValue : playersArray) {
        playerNames.append(playerValue.toObject()["name"].toString());
    }

    // Передаем созданную игру в GameWindow
    gameWindow = new GameWindow(playerNames.size(), 0, true, playerNames, networkGame, this);
    gameWindow->setClient(client);
    gameWindow->setClientPlayerName(client->getPlayerName()); // Передаем имя клиента для определения положения на игровом экране
    qDebug() << "Client player name:" << client->getPlayerName();
    qDebug() << "Game players:" << playerNames;

   // gameWindow->loadGameState(state);
    gameWindow->show();
    this->hide();
}

void MainWindow::onNetworkError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка сети", message);

    if (waitingDialog) {
        waitingDialog->close();
        delete waitingDialog;
        waitingDialog = nullptr;
    }
}

void MainWindow::showWaitingDialog()
{
    waitingDialog = new WaitingDialog(this);
    connect(waitingDialog, &WaitingDialog::canceled, this, [this]() {
        if (client) {
            client->disconnectFromHost();
            delete client;
            client = nullptr;
        }
        waitingDialog->close();
        delete waitingDialog;
        waitingDialog = nullptr;
    });
    waitingDialog->show();
}

bool MainWindow::highlightEnabled() const {
    return isHighlightEnabled;
}

void MainWindow::onRulesClicked() {
    if (!ruleswindow) {
        ruleswindow = new RulesWindow(); // Создаем окно с правилами

        connect(ruleswindow, &RulesWindow::returnToMainMenu, this, [this]() {
            ruleswindow->close();
            this->show();
        });
    }
    this->hide();
    ruleswindow->show();
}

void MainWindow::showSettingsWindow() {
    if (!settingsWindow) {
        settingsWindow = new SettingsWindow();

        // Подключение сигналов SettingsWindow
        connect(settingsWindow, &SettingsWindow::settingsChanged,
                this, &MainWindow::applySettings);

        connect(settingsWindow, &SettingsWindow::returnToMainMenu, this, [this]() {
            settingsWindow->close();
            this->show();  // Возврат в главное меню
        });
        connect(settingsWindow, &SettingsWindow::settingsChanged,
                this, [this](int result) {
                    if (result == QDialog::Rejected) {
                        this->show();
                    }
                });
    }

    this->hide();
    settingsWindow->show();  // Показываем окно настроек
}

void MainWindow::onStatsClicked() {
    if (!statsWindow) {
        statsWindow = new StatisticsWindow();
        connect(statsWindow, &StatisticsWindow::returnToMainMenu, this, [this]() {
            statsWindow->close();
            this->show();
        });
    }
    this->hide();
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/stats.json";
    statsWindow->loadStatistics(filePath); // Путь к JSON-файлу
    statsWindow->show();

}
void MainWindow::applySettings(int players, int bots, bool showReserve, bool soundEnabled, bool highlight) {
    // Сохраняем настройки, которые не требуют перезапуска
    QSettings settings("MyCompany", "DominoGame");

    // Проверяем, изменились ли параметры, требующие перезапуска
    if (players != currentPlayers || bots != currentBots) {
        if (checkCurrentGame(SettingType::Critical)){
            // Сохранить настройки и пересоздать игру
            settings.setValue("showReserve", showReserve);
            settings.setValue("soundEnabled", soundEnabled);
            settings.setValue("highlight", highlight);

            settings.setValue("players", players);
            settings.setValue("bots", bots);
            currentPlayers = players;
            currentBots = bots;

            // Пересоздаем игру только если она активна
            if (gameWindow && !gameWindow->isGameOver()) {
                delete gameWindow;
                gameWindow = new GameWindow(players, bots, showReserve, names, existingGame);
                gameWindow->setHighlightEnabled(highlight);
                connect(gameWindow, &GameWindow::returnToMainMenu, this, &MainWindow::show);
                gameWindow->show();
            }
        }
    }
    else{
        if (checkCurrentGame(SettingType::NonCritical)){
            // Сохранить настройки и продолжить игру
            settings.setValue("showReserve", showReserve);
            settings.setValue("soundEnabled", soundEnabled);
            settings.setValue("highlight", highlight);
        }
    }

    soundEnabled = settings.value("soundEnabled", true).toBool();
    if (soundEnabled) {
        backgroundMusic->play();
    } else {
        backgroundMusic->stop();
    }
}

bool MainWindow::checkCurrentGame(SettingType settingType) {
    if (gameWindow && !gameWindow->isGameOver()) {
        if (settingType == SettingType::Critical) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Подтверждение",
                "Изменение игроков требует перезапуска!\nПерезапустить игру?",
                QMessageBox::Yes | QMessageBox::No
                );

            if (reply == QMessageBox::Yes) {
                delete gameWindow;
                gameWindow = nullptr;
                return true; // Разрешить изменения
            }
            return false; // Отменить изменения
        }
        else {
            // Для некритических настроек сразу сохраняем изменения
            applyNonCriticalSettings();
            return true;
        }
    }
    return true;
}

void MainWindow::applyNonCriticalSettings() {
    if (gameWindow) {
        bool showReserve, soundEnabled, highlight;
        QSettings settings("MyCompany", "DominoGame");
        settings.setValue("showReserve", showReserve);
        settings.setValue("soundEnabled", soundEnabled);
        settings.setValue("highlight", highlight);

        // Передаем текущие настройки в игровое окно
        if (soundEnabled) {
            backgroundMusic->play();
        } else {
            backgroundMusic->stop();
        }
        gameWindow->setHighlightEnabled(highlight);
        gameWindow->setReserveVisible(showReserve);
    }
}

void MainWindow::saveCurrentSettings() {
    if (settingsWindow) {
        settingsWindow->loadSettings();
    }
}
