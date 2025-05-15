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

MainWindow ::MainWindow (QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Настройка главного окна
    setWindowTitle(tr("Игра Домино"));
    ui->setupUi(this);
    soundEffect = new QSoundEffect(this);
    tileSelectSound = new QSoundEffect(this);
    tilePlaceSound = new QSoundEffect(this);

    soundEffect->setSource(QUrl::fromLocalFile("C:/Users/Sopha/Downloads/tech_prog/domino/audio/tap.wav"));
    // tileSelectSound->setSource(QUrl::fromLocalFile("C:/Users/Sopha/Downloads/tech_prog/domino/audio/select.wav"));
    // tilePlaceSound->setSource(QUrl::fromLocalFile("C:/Users/Sopha/Downloads/tech_prog/domino/audio/place.wav"));

    // Фоновая музыка
    audioOutput = new QAudioOutput(this);
    backgroundMusic = new QMediaPlayer(this);
    backgroundMusic->setAudioOutput(audioOutput);
    backgroundMusic->setSource(QUrl::fromLocalFile("C:/Users/Sopha/Downloads/tech_prog/domino/audio/1tema.mp3"));
    audioOutput->setVolume(0.1);
    connect(backgroundMusic, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "Ошибка воспроизведения:" << errorString;
    });

    connect(backgroundMusic, &QMediaPlayer::mediaStatusChanged, this, [](QMediaPlayer::MediaStatus status) {
        qDebug() << "Статус медиа:" << status;
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
    /*QDialog startDialog(this);
    QVBoxLayout layout(&startDialog);

    QPushButton newGameBtn("Новая игра");
    QPushButton loadGameBtn("Загрузить игру");

    connect(&newGameBtn, &QPushButton::clicked, [&]() {
        startNewGame();
        startDialog.accept();
    });

    connect(&loadGameBtn, &QPushButton::clicked, [&]() {
        if(loadSavedGame()) {
            startDialog.accept();
        }
    });

    layout.addWidget(&newGameBtn);
    layout.addWidget(&loadGameBtn);

    if(startDialog.exec() == QDialog::Accepted) {
        this->hide();
        gameWindow->show();
    }*/
    QSettings settings("MyCompany", "DominoGame");
    int players = settings.value("players", 2).toInt();
    int bots = settings.value("bots", 0).toInt();
    bool showReserve = settings.value("showReserve", false).toBool();
    // Если игра завершена или не существует, создать новую
    if (!gameWindow || gameWindow->isGameOver()) {
        delete gameWindow; // Удалить старую игру


        gameWindow = new GameWindow(players, bots, showReserve);
        connect(gameWindow, &GameWindow::returnToMainMenu, this, &MainWindow::show);
     }
    if (settings.value("soundEnabled", true).toBool()) {
        backgroundMusic->play();
    }
     this->hide();
     gameWindow->show();
}

void MainWindow::startNewGame() {
    QSettings settings("MyCompany", "DominoGame");
    gameWindow = new GameWindow(
        settings.value("players", 2).toInt(),
        settings.value("bots", 0).toInt(),
        settings.value("showReserve", true).toBool()
        );
    gameWindow->setIsNewGame(true);
}

bool MainWindow::loadSavedGame() {
    QString filename = "C:/Users/Sopha/Downloads/tech_prog/domino/my_save.sav";
    if(filename.isEmpty()) return false;

    try {
        gameWindow = new GameWindow(2,0, true);
        gameWindow->game->loadGame(filename);  // Метод загрузки
        gameWindow->setIsNewGame(false);
        return true;
    } catch(...) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить игру");
        return false;
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
    if (!checkCurrentGame()) {
        return; // Отмена изменений
    }


    QSettings settings("MyCompany", "DominoGame");
    settings.setValue("players", players);
    settings.setValue("bots", bots);
    settings.setValue("showReserve", showReserve);
    settings.setValue("soundEnabled", soundEnabled);
    settings.setValue("highlight", highlight);

    currentPlayers = players;
    currentBots = bots;
    if (soundEnabled) {
        backgroundMusic->play();
    } else {
        backgroundMusic->stop();
    }
    if (gameWindow) {
        delete gameWindow;
        gameWindow = new GameWindow(players, bots, showReserve);
        connect(gameWindow, &GameWindow::returnToMainMenu, this, &MainWindow::show);
    }

    soundEffect->setMuted(!soundEnabled);
}

bool MainWindow::checkCurrentGame() {
    QSettings settings("MyCompany", "DominoGame");
    int newPlayers = settings.value("players", 2).toInt();
    int newBots = settings.value("bots", 0).toInt();

    if (gameWindow && !gameWindow->isGameOver() &&
        (newPlayers != currentPlayers || newBots != currentBots))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Подтверждение",
                                      "Изменение количества игроков/ботов требует перезапуска игры!\n"
                                      "Вы уверены, что хотите продолжить?",
                                      QMessageBox::Yes|QMessageBox::No);
        return (reply == QMessageBox::Yes);
    }
    return true;
}

void MainWindow::saveCurrentSettings() {
    if (settingsWindow) {
        settingsWindow->loadSettings();
    }
}
