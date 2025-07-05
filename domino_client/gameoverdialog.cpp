#include "gameoverdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "dominolabel.h"
#include "ui_gameoverdialog.h"

GameOverDialog::GameOverDialog(const QVector<Player*>& players, QWidget* parent)
    : QDialog(parent) {
    ui.setupUi(this); // Загрузка UI из файла
    // Настройки окна
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowFullscreenButtonHint);
    setFixedSize(size());
    // Добавление информации об игроках
    for (Player* player : players) {
        QWidget* playerWidget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(playerWidget);

        // Заголовок с именем и счетом
        QLabel* nameLabel = new QLabel(
            QString("%1: %2 очков").arg(player->getName()).arg(player->getScore())
            );
        layout->addWidget(nameLabel);

        // Доминошки игрока
        QWidget* tilesContainer = new QWidget;
        QHBoxLayout* tilesLayout = new QHBoxLayout(tilesContainer);
        for (const DominoTile& tile : player->getHand()) {
            DominoLabel* label = new DominoLabel;
            label->setFixedSize(40, 80);
            label->setStyleSheet("background: white; border: 1px solid black;");
            label->setDots(tile.getLeftValue(), tile.getRightValue(), true, true);
            tilesLayout->addWidget(label);
        }
        layout->addWidget(tilesContainer);

        ui.playersLayout->addWidget(playerWidget);
    }


    // Создаем контейнер для кнопок
    QWidget* buttonContainer = new QWidget;
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);

    // Кнопка "Новый раунд"
    QPushButton* newRoundButton = new QPushButton("Новый раунд");
    buttonLayout->addWidget(newRoundButton);

    // Кнопка "Выйти в меню"
    QPushButton* exitButton = new QPushButton("Выйти в меню");
    buttonLayout->addWidget(exitButton);

    // Добавляем контейнер с кнопками в основной макет
    ui.verticalLayout->addWidget(buttonContainer);

    QPushButton* closeBtn = new QPushButton("×", this);
    closeBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 18px;
            border: none;
            color: #444;
            qproperty-alignment: AlignRight;
        }
        QPushButton:hover {
            color: #ff4444;
        }
    )");
    closeBtn->setFixedSize(24, 24);
    closeBtn->move(width() - 34, 6);
    connect(closeBtn, &QPushButton::clicked, this, &GameOverDialog::close);

    connect(newRoundButton, &QPushButton::clicked, this, &GameOverDialog::newRoundRequested);
    connect(exitButton, &QPushButton::clicked, this, &GameOverDialog::exitToMainMenu);
}
