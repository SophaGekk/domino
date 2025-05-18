#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "domino_game.h"
#include "domino_tile_item.h"
#include "player.h"
#include <QMessageBox>
#include <QTime>
#include "statisticswindow.h"
#include <QFileDialog>
#include <QShortcut>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "nicknamedialog.h"

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
    backgroundMusic->setSource(QUrl::fromLocalFile("C:/Users/Sopha/Downloads/tech_prog/domino/audio/1tema.mp3"));
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
        gameWindow = new GameWindow(players, bots, showReserve, names);
        gameWindow->setHighlightEnabled(settings.value("highlight", false).toBool());
        connect(gameWindow, &GameWindow::returnToMainMenu, this, &MainWindow::show);

        if (settings.value("soundEnabled", true).toBool()) {
            backgroundMusic->play();
        }

        gameWindow->show();
        this->hide();
    }
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
    statsWindow->loadStatistics("C:/Users/Sopha/Downloads/tech_prog/domino/stats.json"); // Путь к JSON-файлу
    statsWindow->show();

}

void MainWindow::applySettings(int players, int bots, bool showReserve, bool soundEnabled, bool highlight) {
    // Сохраняем настройки, которые не требуют перезапуска
    QSettings settings("MyCompany", "DominoGame");
    settings.setValue("showReserve", showReserve);
    settings.setValue("soundEnabled", soundEnabled);
    settings.setValue("highlight", highlight);

    // Динамическое обновление звука
    //soundEffect->setMuted(!soundEnabled);
    if (soundEnabled) {
        backgroundMusic->play();
    } else {
        backgroundMusic->stop();
    }

    // Проверяем, изменились ли параметры, требующие перезапуска
    if (players != currentPlayers || bots != currentBots) {
        if (!checkCurrentGame()) return;

        settings.setValue("players", players);
        settings.setValue("bots", bots);
        currentPlayers = players;
        currentBots = bots;

        // Пересоздаем игру только если она активна
        if (gameWindow && !gameWindow->isGameOver()) {
            delete gameWindow;
            gameWindow = new GameWindow(players, bots, showReserve, names);
            gameWindow->setHighlightEnabled(highlight);
            connect(gameWindow, &GameWindow::returnToMainMenu, this, &MainWindow::show);
            gameWindow->show();
        }
    }
}
bool MainWindow::checkCurrentGame() {
    if (gameWindow && !gameWindow->isGameOver()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Подтверждение",
            "Изменение настроек требует перезапуска игры!\nВы уверены?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            delete gameWindow;
            gameWindow = nullptr;
            return true;
        }
        return false;
    }
    return true;
}
void MainWindow::saveCurrentSettings() {
    if (settingsWindow) {
        settingsWindow->loadSettings();
    }
}
