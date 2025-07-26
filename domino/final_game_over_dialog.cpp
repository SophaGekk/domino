#include "final_game_over_dialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>

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

    // Зеленая цветовая схема
    setStyleSheet("background-color: #e8f5e9; border-radius: 15px; padding: 25px;");

    // Увеличиваем минимальный размер для 4 игроков
    setMinimumSize(600, 600);

    // Заголовок с зеленым градиентом
    QLabel* titleLabel = new QLabel("🎮 Игра Завершена! 🏆");
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "padding: 15px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4CAF50, stop:1 #2E7D32);"
        "border-radius: 10px;"
        "color: white;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(15);

    // Определение победителей
    QVector<int> winnerIndices;
    for (int i = 0; i < players.size(); ++i) {
        if (totalScores[i] >= maxScore) {
            winnerIndices.append(i);
        }
    }

    // Сообщение о результатах
    QLabel* resultLabel = new QLabel;
    resultLabel->setStyleSheet("font-size: 18px; color: #1B5E20; font-weight: bold;");
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setWordWrap(true);

    if (!winnerIndices.isEmpty()) {
        if (isDraw || winnerIndices.size() > 1) {
            QStringList winnerNames;
            for (int idx : winnerIndices) {
                winnerNames.append(players[idx]->getName());
            }
            resultLabel->setText("🏆 Ничья! Победители: " + winnerNames.join(", ") +
                                 "\nНабрали " + QString::number(maxScore) + "+ очков");
        } else {
            Player* winner = players[winnerIndices.first()];
            resultLabel->setText("🎉 Поздравляем, " + winner->getName() + "!\n"
                                                                          "Вы победили с " + QString::number(totalScores[winnerIndices.first()]) + " очками!");
        }
    } else {
        resultLabel->setText("Игра завершена!");
    }

    layout->addWidget(resultLabel);
    layout->addSpacing(20);

    // Заголовок таблицы
    QLabel* resultsLabel = new QLabel("🏁 Итоговые результаты:");
    resultsLabel->setStyleSheet("font-size: 20px; color: #2E7D32; font-weight: bold;");
    layout->addWidget(resultsLabel);
    layout->addSpacing(10);

    // Таблица результатов
    QTableWidget* table = new QTableWidget(players.size(), 2);
    table->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 10px;"
        "   gridline-color: #c8e6c9;"
        "   font-size: 14px;"
        "   alternate-background-color: #f1f8e9;"
        "}"

        );

    table->setHorizontalHeaderLabels(QStringList() << "Игрок" << "Очки");
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->setAlternatingRowColors(true);

    // Увеличиваем высоту строк для лучшей читаемости
    table->verticalHeader()->setDefaultSectionSize(40);

    // Сортируем игроков по очкам (от большего к меньшему)
    QVector<QPair<int, int>> sortedPlayers;
    for (int i = 0; i < players.size(); ++i) {
        sortedPlayers.append(qMakePair(totalScores[i], i));
    }
    std::sort(sortedPlayers.begin(), sortedPlayers.end(),
              [](const QPair<int, int>& a, const QPair<int, int>& b) {
                  return a.first > b.first;
              });

    // Заполняем таблицу
    for (int i = 0; i < sortedPlayers.size(); ++i) {
        int playerIdx = sortedPlayers[i].second;
        Player* player = players[playerIdx];
        int score = totalScores[playerIdx];

        QTableWidgetItem* nameItem = new QTableWidgetItem(player->getName());
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(score));

        // Выделяем победителей
        if (score >= maxScore) {
            QFont winnerFont = nameItem->font();
            winnerFont.setBold(true);
            winnerFont.setPointSize(16);
            nameItem->setFont(winnerFont);
            scoreItem->setFont(winnerFont);

            nameItem->setForeground(QColor(46, 125, 50)); // Темно-зеленый
            scoreItem->setForeground(QColor(46, 125, 50));

            nameItem->setBackground(QColor(232, 245, 233)); // Светло-зеленый
            scoreItem->setBackground(QColor(232, 245, 233));
        } else {
            QFont regularFont = nameItem->font();
            regularFont.setPointSize(14);
            nameItem->setFont(regularFont);
            scoreItem->setFont(regularFont);
        }

        table->setItem(i, 0, nameItem);
        table->setItem(i, 1, scoreItem);
    }

    layout->addWidget(table);
    layout->addSpacing(25);

    // Кнопка выхода в зеленой гамме
    QPushButton* exitButton = new QPushButton("🚪 Выход в меню");
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   border-radius: 10px;"
        "   color: white;"
        "   padding: 15px 30px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #388E3C;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1B5E20;"
        "}"
        );
    exitButton->setCursor(Qt::PointingHandCursor);
    connect(exitButton, &QPushButton::clicked, this, [this]() {
        emit exitToMainMenu();
        close();
    });
    layout->addWidget(exitButton, 0, Qt::AlignCenter);

    setLayout(layout);
}
