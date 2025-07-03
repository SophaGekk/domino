#include "final_game_over_dialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>

FinalGameOverDialog::FinalGameOverDialog(
    const QVector<Player*>& players,
    const QVector<int>& totalScores,
    int maxScore,
    bool isDraw,
    QWidget* parent
    ) : QDialog(parent), players(players), totalScores(totalScores), maxScore(maxScore), isDraw(isDraw) {
    setupUI();
}

void FinalGameOverDialog::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    setStyleSheet("background-color: white; border-radius: 10px; padding: 20px;");

    // Заголовок
    QLabel* titleLabel = new QLabel("Игра завершена!");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Определяем победителя
    int winnerScore = 0;
    Player* winner = nullptr;
    for (int i = 0; i < players.size(); ++i) {
        if (totalScores[i] > winnerScore) {
            winnerScore = totalScores[i];
            winner = players[i];
        }
    }

    // Сообщение о победителе
    QLabel* winnerLabel;
    if (isDraw) {
        winnerLabel = new QLabel("Ничья! Несколько игроков достигли лимита очков.");
    } else if (winner) {
        winnerLabel = new QLabel("Поздравляем, " + winner->getName() + "!\nВы достигли " +
                                 QString::number(maxScore) + " очков и победили!");
    } else {
        winnerLabel = new QLabel("Игра завершена!");
    }
    winnerLabel->setStyleSheet("font-size: 18px;");
    winnerLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(winnerLabel);

    // Таблица результатов
    QLabel* resultsLabel = new QLabel("Итоговые очки:");
    resultsLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(resultsLabel);

    QTableWidget* table = new QTableWidget(players.size(), 2);
    table->setHorizontalHeaderLabels(QStringList() << "Игрок" << "Очки");
    // table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < players.size(); ++i) {
        QTableWidgetItem* nameItem = new QTableWidgetItem(players[i]->getName());
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(totalScores[i]));

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, scoreItem);
    }
    table->resizeColumnsToContents();
    layout->addWidget(table);

    // Кнопка выхода
    QPushButton* exitButton = new QPushButton("Выход в меню");
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   text-align: center;"
        "   text-decoration: none;"
        "   font-size: 16px;"
        "   margin: 10px 2px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        );
    connect(exitButton, &QPushButton::clicked, this, [this]() {
        emit exitToMainMenu();
        close();
    });
    layout->addWidget(exitButton);

    setLayout(layout);
}
