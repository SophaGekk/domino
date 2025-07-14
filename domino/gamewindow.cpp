#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include "bazaar.h"
#include "player.h"
#include "dominolabel.h"
#include "bot_player.h"
#include "clickable_rect.h"
#include <QGraphicsProxyWidget>
#include "ClickableLabel.h"
#include "gameoverdialog.h"
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QToolTip>
#include "chatmanager.h"
#include "final_game_over_dialog.h"
#include "client.h"
#include "HumanPlayer.h"

GameWindow::GameWindow(int playersCount, int BotCount, bool isShowReserve, const QStringList &playerNames, DominoGame* existingGame, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWindow), playersCount(playersCount), BotCount(BotCount), playerNames(playerNames) {
    ui->setupUi(this);
    m_spaceKeyEnabled = false;
    gameOverShown = false;
    showReserve = isShowReserve;
    // Настройки окна
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowFullscreenButtonHint);
    setFixedSize(size()); // Фиксирует размер после инициализации интерфейса
    scene = new QGraphicsScene(this);
    ui->dominoArea->setScene(scene);

    // Инициализация игры
    if (existingGame) {
        isNetworkGame = true;
        game = existingGame;
        game->setParent(this);
        connect(client, &Client::gameOver, this, &GameWindow::handleNetworkGameOver);


    } else {
        isNetworkGame = false;
        // Режим локальной игры: создаем новую игру
        game = new DominoGame(playersCount, BotCount, playerNames, this);
    }

    m_turnOverlay = new QWidget(this);

    // Настройка кнопки "Домой"
    connect(ui->homeButton, &QPushButton::clicked, this, &GameWindow::onHomeClicked);

    // Обновление интерфейса
    connect(game, &DominoGame::playerChanged, this, &GameWindow::onPlayerChanged);
    connect(game, &DominoGame::gameStarted, this, &GameWindow::updateGameState);
    connect(game, &DominoGame::playerMoved, this, &GameWindow::updateGameState);
    connect(game, &DominoGame::RoundEnded, this, &GameWindow::showGameOver);
    connect(game, &DominoGame::gameFinish, this, &GameWindow::showFinalGameOver);

    reserveButton = new QPushButton(this);
    reserveButton->setObjectName("reserveButton");
    ui->labelReserveLayout->addWidget(reserveButton); // Добавьте кнопку в layout
    connect(reserveButton, &QPushButton::clicked, this, &GameWindow::onReserveClicked);

    ui->scrollBottomHand->setStyleSheet("QScrollBar:horizontal { height: 12px; }");
    ui->scrollDominoArea->setStyleSheet("QScrollArea { border: none; }");
    ui->dominoArea->setAlignment(Qt::AlignCenter); // Центрирование содержимого


    m_turnOverlay->setStyleSheet("background: rgba(0, 0, 0, 320);");
    m_turnOverlay->hide();
    m_turnLabel = new QLabel(m_turnOverlay);
    m_turnLabel->setAlignment(Qt::AlignCenter);
    m_turnLabel->setStyleSheet("font-size: 32px; color: white;");
    m_turnLabel->setGeometry(0, 0, width(), height());
    m_turnOverlay->hide();

    // Создание виджетов чата
    chatManager = new ChatManager(this);
    chatHistoryWidget = new QTextEdit(this);
    chatHistoryWidget->setReadOnly(true);
    chatHistoryWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget* inputContainer = new QWidget(this);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputContainer);
    inputLayout->setContentsMargins(0, 0, 0, 0); // Убрать отступы

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("➤", this); // Иконка стрелки
    sendButton->setFixedSize(40, 30); // Фиксированный размер кнопки

    inputLayout->addWidget(messageInput);  // Поле ввода растягивается
    inputLayout->addWidget(sendButton);    // Кнопка фиксирована справа

    // Вертикальный макет для всего чата
    QVBoxLayout* chatLayout = new QVBoxLayout;
    chatLayout->addWidget(new QLabel("Чат"));
    chatLayout->addWidget(chatHistoryWidget);
    chatLayout->addWidget(inputContainer); // Добавляем контейнер с полем ввода и кнопкой

    ui->rightPanelLayout->addLayout(chatLayout);

    chatHistoryWidget->setStyleSheet("QTextEdit { background: #f0f0f0; border-radius: 5px; padding: 5px; }");
    messageInput->setStyleSheet("QLineEdit { border: 1px solid #ccc; border-radius: 3px; padding: 5px; }");
    sendButton->setStyleSheet("QPushButton { background: #4CAF50; color: white; border: none; padding: 8px; border-radius: 3px; }" "QPushButton:hover { background: #45a049; }");

    // Подключение сигналов
    connect(sendButton, &QPushButton::clicked, this, &GameWindow::sendChatMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &GameWindow::sendChatMessage);
    connect(chatManager, &ChatManager::newMessage, this, &GameWindow::updateChatHistory);

    connect(ui->pushButton_skip, &QPushButton::clicked, this, &GameWindow::on_pushButton_skip_clicked);

    updateGameState();
    setupHands();
    updateUI();
}


void GameWindow::updateGameState() {
    if (game->isGameOver()) {
        int winnerIndex = game->determineWinner();
        Player* winner = game->getPlayers()[winnerIndex];

        // Блокируем дальнейшие действия для ботов
        game->blockSignals(true);

        // Показываем сообщение только один раз
        if (!gameOverShown) {
            gameOverShown = true;
            if (winnerIndex == -1) {
                QMessageBox::information(this, "Игра окончена", "Ничья");
            } else {
                QMessageBox::information(this, "Игра окончена", "Победитель: " + winner->getName());
            }

        }
        return;
    }

    // Обновление информации о раунде
    ui->labelRound->setText("Раунд: " + QString::number(game->getCurrentRound()));
    // Обновление резерва
    updateReserveLabel(showReserve);

    // Обновление информации об игроках
    const auto& players = game->getPlayers();
    ui->labelNameBottom->setText(game->getCurrentPlayer()->getName());
    ui->labelScoreBottom->setText(QString::number(game->getCurrentPlayer()->getScore()));
    ui->avatarBottom->setPixmap(game->getCurrentPlayer()->getAvatar()); // Установка аватарки

    game->updateNextPlayerIndex();
    if(players.size() >= 3)
    {
        game->updateThirdPlayerIndex();
        // правый бок
        ui->labelNameRight->setText(game->getNextPlayer()->getName());
        ui->labelScoreRight->setText(QString::number(game->getNextPlayer()->getScore()));
        //ui->avatarRight->setPixmap(game->getNextPlayer()->getAvatar());

        //верхний
        ui->labelNameTop->setText(game->getThirdPlayer()->getName());
        ui->labelScoreTop->setText(QString::number(game->getThirdPlayer()->getScore()));
        ui->avatarTop->setPixmap(game->getThirdPlayer()->getAvatar());

        if(players.size() == 4)
        {
            game->updateFourthPlayerIndex();
            // левый бок
            ui->labelNameLeft->setText(game->getFourthPlayer()->getName());
            ui->labelScoreLeft->setText(QString::number(game->getFourthPlayer()->getScore()));
            //ui->avatarLeft->setPixmap(game->getFourthPlayer()->getAvatar());
        }
    }
    else
    {
        //верхний
        ui->labelNameTop->setText(game->getNextPlayer()->getName());
        ui->labelScoreTop->setText(QString::number(game->getNextPlayer()->getScore()));
        ui->avatarTop->setPixmap(game->getNextPlayer()->getAvatar());
    }


    updateHandDisplay();
    // Подсветка допустимых ходов
    if (isHighlightEnabled && !game->getCurrentPlayer()->isBot() && !game->isGameOver()) {
        highlightValidTiles();

    } else {
        clearHighlights();
    }

    ui->pushButton_skip->setEnabled(!game->getCurrentPlayer()->isBot() && !game->currentPlayerCanMove() && game->getBazaar()->isEmpty()); // Активируем кнопку, если ходов нет
}

void GameWindow::sendChatMessage() {
    QString message = messageInput->text().trimmed();
    if (!message.isEmpty()) {
        if (isNetworkGame) {
            client->sendChatMessage(message);
        } else {
            QString playerName = game->getCurrentPlayer()->getName();
            chatManager->sendMessage(playerName, message);
        }
        messageInput->clear();
    }
}



void GameWindow::updateChatHistory(const QString& formattedMessage) {
    chatHistoryWidget->append(formattedMessage);
    if (!isActiveWindow()) {
        QApplication::alert(this); // Мигание иконки в таскбаре
    }
}

void GameWindow::clearBoard() {
    scene->clear();
}

void GameWindow::onHomeClicked() {

    emit returnToMainMenu();
    m_turnOverlay->hide();
    this->hide();

}

GameWindow::~GameWindow() {
    delete ui;
    delete game;
    delete scene;
}


void GameWindow::setHighlightEnabled(bool highlight) {
    isHighlightEnabled = highlight;
}

void GameWindow::setPlayersCount(int count) {
    currentPlayersCount = count;
}

void GameWindow::setBotCount(int count) {
    BotCount = count;
}


void GameWindow::setupHands() {
    for (DominoLabel* label : bottomHandLabels) {
        connect(label, &DominoLabel::clicked, this, [this, label]() {
            handleDominoClick(label), isValidMessage = false;
        });
    }
}

void GameWindow::handleDominoClick(DominoLabel* clickedLabel) {
    // Сброс предыдущей подсветки
    clearHighlights();
    clearSelection();

    selectedTile = getTileFromLabel(clickedLabel);
    clickedLabel->setHighlighted(true);

    // Проверка возможности хода
    DominoTile tile = getTileFromLabel(clickedLabel);

    QVector<int> ends = game->getBoardEnds();

    bool isValid = false;

    if (ends.isEmpty()) {
        // Первый ход: можно положить дубль
        bool isDouble = tile.isDouble() && (tile.getLeftValue() != 0);
        bool isMaxTile = (tile == game->getMaxTileWithoutDoubles());
        // Если у кого-то есть дубль, разрешаем только дубли (кроме 0-0)
        if (game->getHasDouble()) {
            isValid = isDouble;
        }
        // Если дублей нет, разрешаем самую "дорогую" костяшку
        else {
            isValid = isMaxTile;
        }
        qDebug() << "Первый ход:" << tile.getLeftValue() << "|" << tile.getRightValue();
    }
    else {
        isValid = (tile.getLeftValue() == ends.first() || tile.getLeftValue() == ends.last() ||
                   tile.getRightValue() == ends.first() || tile.getRightValue() == ends.last());
        qDebug() << "Ход:" << tile.getLeftValue() << "|" << tile.getRightValue();
        qDebug() << "Концы стола:" << ends.first() << "|" << ends.last();
    }

    if (isValid) {
        showPossibleMoves(tile);
    } else {
        if(!isValidMessage)
        {
            isValidMessage = true;
            QMessageBox::warning(this, "Ошибка", "Невозможно сделать ход этой костяшкой.");

        }
        clearHighlights();
        clearSelection();
        return;
    }

}


void GameWindow::processMove(DominoTile tile) {
    if (game->isGameOver()) return;
    while (game->getCurrentPlayer()->isBot() && !game->isGameOver()) {
        // Получаем текущие концы доски
        QVector<int> ends = game->getBoardEnds();
        int leftEnd;
        int rightEnd;
        if(ends.isEmpty())
        {
            leftEnd = -1;
            rightEnd = -1;
        }
        else
        {
            leftEnd = ends.first();
            rightEnd = ends.last();
        }

        auto [botTile, isBotLeftEnd] = game->getCurrentPlayer()->playTurn( leftEnd, rightEnd );

        if (botTile.isValidNumer()) {
            game->getCurrentPlayer()->removeTile(botTile);
            if(!ends.isEmpty())
            {
               game->placeTile(botTile, isBotLeftEnd);
            }
            else{game->getBoard().append(botTile);}
            game->makeMove();
            updateGameState();
            updateBoard();
            QCoreApplication::processEvents(); // Обновляем GUI
        } else {
            // Логика для взятия из базара
            if (!game->getBazaar()->isEmpty()) {
                DominoTile newTile = game->getBazaar()->drawTile();
                game->getCurrentPlayer()->addTile(newTile);
                game->getBazaar()->removeTile(newTile);
                // Обновить интерфейс
                ui->labelReserve->setText("Резерв: " + QString::number(game->getBazaar()->remainingTilesCount()));

            }
            else {game->makeMove();}
        }
    }

}

void GameWindow::showPossibleMoves(const DominoTile& tile) {
    if (game->getCurrentPlayer()->isBot() || !tile.isValidNumer()) return;
    QVector<int> ends = game->getBoardEnds();

    // Если стол пуст, подсветить центр для первого хода
    if (ends.isEmpty()) {
        // Координаты центра игрового поля (пример)
        int centerX = ui->dominoArea->width() / 2-30;
        int centerY = ui->dominoArea->height() / 2-15;
        QPoint centerPosition(centerX, centerY);

        // Создать прямоугольник с контуром
        ClickableRect* rect = new ClickableRect();
        rect->setRect(centerX, centerY, 30, 70);
        rect->setPen(QPen(Qt::red, 2));
        rect->setData(0, "first_move"); // Пометить элемент для обработки
        connect(rect, &ClickableRect::clicked, this, &GameWindow::onFirstMoveClicked);
        scene->addItem(rect);
        return;
    }

    else {
        // Подсветка для левого и правого концов
        QPoint leftPos = leftEndPosition - getTilePosition(game->getBoard().first());
        QPoint rightPos = rightEndPosition + getTilePosition(game->getBoard().last());

        // Проверка возможности хода слева
        if (tile.getRightValue() == ends.first() || tile.getLeftValue() == ends.first()) {
            drawArrow(leftPos, true, ends.first());
        }

        // Проверка возможности хода справа
        if (tile.getLeftValue() == ends.last() || tile.getRightValue() == ends.last()) {
            drawArrow(rightPos, false, ends.last());
        }
    }
}



void GameWindow::onFirstMoveClicked() {
    DominoTile selectedTile = getSelectedTile();
    if (selectedTile.isValidNumer()) {
        handleMove(selectedTile, true); // true - условно для первого хода
    }
}

// Обработчик клика на левый конец доски
void GameWindow::onLeftEndClicked() {
    DominoTile selectedTile = getSelectedTile();
    if (selectedTile.isValidNumer()) {
        handleMove(selectedTile, true); // true = левый конец
    }
}

// Обработчик клика на правый конец доски
void GameWindow::onRightEndClicked() {
    DominoTile selectedTile = getSelectedTile();
    if (selectedTile.isValidNumer()) {
        handleMove(selectedTile, false); // false = правый конец
    }
}

void GameWindow::handleMove(DominoTile tile, bool isLeftEnd) {
    if (isNetworkGame) {
        client->sendMove(tile, isLeftEnd); // Отправка на сервер
        clearSelection();
    } else {
        if (game->getBoard().isEmpty()) {// Первый ход
            game->getBoard().append(tile);
        } else {
            QVector<int> ends = game->getBoardEnds();
            if (isLeftEnd) {
                if (tile.getLeftValue() == ends.first()) tile.flip();
                game->placeTile(tile, true);
            } else {
                if (tile.getRightValue() == ends.last()) tile.flip();
                game->placeTile(tile, false);
            }
        }

        // Общая логика для всех ходов
        game->getCurrentPlayer()->removeTile(tile);
        updateBoard();
        updateHandDisplay();
        game->makeMove();
        updateGameState();
        clearSelection();
    }
}

DominoTile GameWindow::getTileFromLabel(DominoLabel* label) {
    for (int i = 0; i < bottomHandLabels.size(); ++i) {
        if (bottomHandLabels[i] == label) {
            return game->getCurrentPlayer()->getHand()[i];
        }
    }
    return DominoTile(-1, -1); // Ошибка, если не найдено
}

void GameWindow::updateBoard() {
    scene->clear();

    const QVector<DominoTile>& board = game->getBoard();

    // Определить позицию для первой домино (центр)
    int currentX = ui->dominoArea->width() / 2 - 30;
    int currentY = ui->dominoArea->height() / 2 - 15;


    for (const DominoTile& tile : board) {
        bool isDouble = (tile.getLeftValue() == tile.getRightValue());
        int width = isDouble ? 40 : 80;   // Вертикальный (дубль) или горизонтальный
        int height = isDouble ? 80 : 40;

        DominoLabel *label = new DominoLabel();
        label->setDots(tile.getLeftValue(), tile.getRightValue(), isDouble, true); // Передать значения точек
        if(isDouble)
        {
            currentY = ui->dominoArea->height() / 2 - 15;
            label->setFixedSize(width, height); // Размер домино (ширина x высота)
            // Добавить виджет на сцену через QGraphicsProxyWidget
            QGraphicsProxyWidget *proxy = scene->addWidget(label);
            proxy->setPos(currentX, currentY); // Установить позицию
            currentX += 40;

        }
        else{
            currentY = ui->dominoArea->height() / 2 + 10;
            label->setFixedSize(width, height); // Размер домино (ширина x высота)
            // Добавить виджет на сцену через QGraphicsProxyWidget
            QGraphicsProxyWidget *proxy = scene->addWidget(label);
            proxy->setPos(currentX, currentY); // Установить позицию
            currentX += 80;
        }

    }
}

void GameWindow::drawArrow(const QPoint& pos, bool isLeft, const DominoTile& tile) {
    bool isDouble = (tile.getLeftValue() == tile.getRightValue());
    int width = isDouble ? 30 : 70;
    int height = isDouble ? 70 : 30;
    int offset = isDouble ? 70 : 30; // Смещение в зависимости от ориентации

    QPointF highlightPos;
    if (isLeft) {
        highlightPos = QPointF(pos.x() - offset, pos.y());
    } else {
        highlightPos = QPointF(pos.x() + offset, pos.y());
    }

    // Если прямоугольник выходит за пределы экрана, смещаем его
    QRectF visibleArea = ui->dominoArea->mapToScene(ui->dominoArea->viewport()->geometry()).boundingRect();
    if (!visibleArea.contains(highlightPos)) {
        // Смещаем вниз, чтобы был над домино игрока, но не перекрывал стол
        highlightPos.setY(visibleArea.bottom() - 100);
        if (isLeft) {
            highlightPos.setX(visibleArea.left() + 50 + width); // Левый край
        } else {
            highlightPos.setX(visibleArea.right() - 50 - width); // Правый край
        }
    }

    // Создать кликабельный прямоугольник
    ClickableRect* highlight = new ClickableRect();
    highlight->setRect(highlightPos.x(), highlightPos.y(), width, height);
    highlight->setPen(QPen(Qt::red, 2, Qt::DashLine));
    highlight->setBrush(QBrush(QColor(255, 0, 0, 50)));

    // Передать данные о направлении (левый/правый конец)
    highlight->setData(0, QVariant(isLeft));
    // Подключение обработчиков
    if (isLeft) {
        connect(highlight, &ClickableRect::clicked, this, &GameWindow::onLeftEndClicked);
    } else {
        connect(highlight, &ClickableRect::clicked, this, &GameWindow::onRightEndClicked);
    }
    scene->addItem(highlight);
}

void GameWindow::clearSelection() {
    for (DominoLabel* label : bottomHandLabels) {
        label->setHighlighted(false);
    }
    selectedTile = DominoTile(-1, -1); // Сброс выбранной костяшки
}

void GameWindow::handleHighlightClick(bool isLeftEnd) {
    Player* currentPlayer = game->getCurrentPlayer();
    DominoTile selectedTile = currentPlayer->getSelectedTile();

    // Удалить домино из руки игрока
    currentPlayer->removeTile(selectedTile);

    // Добавить домино на доску
    game->placeTile(selectedTile, isLeftEnd);

    // Обновить интерфейс
    updateBoard();
    updateHandDisplay();
    updateHighlightedAreas(); // Обновить подсветку
}

void GameWindow::updateHandDisplay() {
    if (isNetworkGame) {
        const auto& players = game->getPlayers();
        for (Player* player : players) {
            QString playerName = player->getName();
            if (playerPositions.contains(playerName)) {
                int position = playerPositions[playerName];
                bool isOurHand = (playerName == clientPlayerName);
                updateHandForPosition(player, position, isOurHand);
            }
        }
    } else {

        if (game->getCurrentPlayer()->isBot()) {
            if(game->getPlayers().size() == 2 || (game->getPlayers().size() == 3 && BotCount == 1)|| (game->getPlayers().size() == 3 && BotCount == 2 && !game->getNextPlayer()->isBot()) || (game->getPlayers().size() == 4 && ((BotCount == 3 || BotCount == 2) && game->getNextPlayer()->isBot() && !game->getThirdPlayer()->isBot())))
            {
                const auto& topHand = game->getThirdPlayer()->getHand();
                // Удалить лишние элементы
                while (topHandLabels.size() > topHand.size()) {
                    DominoLabel* label = topHandLabels.takeLast();
                    delete label;
                }

                // Добавить новые
                for (int i = topHandLabels.size(); i < topHand.size(); ++i) {
                    DominoLabel* label = new DominoLabel(this);
                    label->setFixedSize(30, 50);
                    label->setStyleSheet("background: white; border: 1px solid black;");
                    ui->horizontalLayoutTopHand->addWidget(label);
                    topHandLabels.append(label);
                }

                // Обновить верхние костяшки (задняя сторона)
                for (int i = 0; i < topHand.size(); ++i) {
                    topHandLabels[i]->setLineVisible(false); // Скрыть точки
                    topHandLabels[i]->show();
                }

            }
            if((game->getPlayers().size() == 3 && BotCount == 2 && game->getNextPlayer()->isBot()) || (game->getPlayers().size() == 4 && BotCount == 3 && game->getNextPlayer()->isBot() && game->getThirdPlayer()->isBot()) )
            {
                const auto& rightHand = game->getNextPlayer()->getHand();;
                // Удалить лишние элементы
                while (rightHandLabels.size() > rightHand.size()) {
                    DominoLabel* label = rightHandLabels.takeLast();
                    delete label;
                }

                // Добавить новые
                for (int i = rightHandLabels.size(); i < rightHand.size(); ++i) {
                    DominoLabel* label = new DominoLabel(this);
                    label->setFixedSize(50, 30);
                    label->setStyleSheet("background: white; border: 1px solid black;");
                    ui->verticalLayoutRightHand->addWidget(label);
                    rightHandLabels.append(label);
                }

                // Обновить верхние костяшки (задняя сторона)
                for (int i = 0; i < rightHand.size(); ++i) {
                    rightHandLabels[i]->setLineVisible(false); // Скрыть точки
                    rightHandLabels[i]->show();
                }

            }
            if(game->getPlayers().size() == 4 && (BotCount == 1 || (BotCount == 2 && !game->getNextPlayer()->isBot()) || (BotCount == 3 && !game->getNextPlayer()->isBot())))
            {
                const auto& leftHand = game->getNextPlayer()->getHand();;
                // Удалить лишние элементы
                while (leftHandLabels.size() > leftHand.size()) {
                    DominoLabel* label = leftHandLabels.takeLast();
                    delete label;
                }

                // Добавить новые
                for (int i = leftHandLabels.size(); i < leftHand.size(); ++i) {
                    DominoLabel* label = new DominoLabel(this);
                    label->setFixedSize(50, 30);
                    label->setStyleSheet("background: white; border: 1px solid black;");
                    ui->verticalLayoutLeftHand->addWidget(label);
                    leftHandLabels.append(label);
                }

                // Обновить верхние костяшки (задняя сторона)
                for (int i = 0; i < leftHand.size(); ++i) {
                    leftHandLabels[i]->setLineVisible(false); // Скрыть точки
                    leftHandLabels[i]->show();
                }
            }

        } else {
            const auto& hand = game->getCurrentPlayer()->getHand();
            // Удалить старые элементы, если их больше, чем нужно
            while (bottomHandLabels.size() > hand.size()) {
                DominoLabel* label = bottomHandLabels.takeLast();
                delete label;
            }
            // Добавить новые элементы, если рука увеличилась
            for (int i = bottomHandLabels.size(); i < hand.size(); ++i) {
                DominoLabel* label = new DominoLabel(this);
                label->setFixedSize(40, 80);
                label->setStyleSheet("background: white; border: 1px solid black;");
                connect(label, &DominoLabel::clicked, this, [this, label]() {
                    handleDominoClick(label);
                });
                ui->horizontalLayoutBottomHand->addWidget(label);
                bottomHandLabels.append(label);
            }

            // Обновить значения для всех костяшек
            for (int i = 0; i < hand.size(); ++i) {
                DominoTile tile = hand[i];
                bottomHandLabels[i]->setDots(tile.getLeftValue(), tile.getRightValue(), true, true);
                bottomHandLabels[i]->show();
            }



            if(game->getPlayers().size() >= 3){
                const auto& topHand = game->getThirdPlayer()->getHand();
                // Удалить лишние элементы
                while (topHandLabels.size() > topHand.size()) {
                    DominoLabel* label = topHandLabels.takeLast();
                    delete label;
                }

                // Добавить новые
                for (int i = topHandLabels.size(); i < topHand.size(); ++i) {
                    DominoLabel* label = new DominoLabel(this);
                    label->setFixedSize(30, 50);
                    label->setStyleSheet("background: white; border: 1px solid black;");
                    ui->horizontalLayoutTopHand->addWidget(label);
                    topHandLabels.append(label);
                }

                // Обновить верхние костяшки (задняя сторона)
                for (int i = 0; i < topHand.size(); ++i) {
                    topHandLabels[i]->setLineVisible(false); // Скрыть точки
                    topHandLabels[i]->show();
                }

                const auto& rightHand = game->getNextPlayer()->getHand();;
                // Удалить лишние элементы
                while (rightHandLabels.size() > rightHand.size()) {
                    DominoLabel* label = rightHandLabels.takeLast();
                    delete label;
                }

                // Добавить новые
                for (int i = rightHandLabels.size(); i < rightHand.size(); ++i) {
                    DominoLabel* label = new DominoLabel(this);
                    label->setFixedSize(50, 30);
                    label->setStyleSheet("background: white; border: 1px solid black;");
                    ui->verticalLayoutRightHand->addWidget(label);
                    rightHandLabels.append(label);
                }

                // Обновить верхние костяшки (задняя сторона)
                for (int i = 0; i < rightHand.size(); ++i) {
                    rightHandLabels[i]->setLineVisible(false); // Скрыть точки
                    rightHandLabels[i]->show();
                }

                if(game->getPlayers().size()==4)
                {
                    const auto& leftHand = game->getNextPlayer()->getHand();;
                    // Удалить лишние элементы
                    while (leftHandLabels.size() > leftHand.size()) {
                        DominoLabel* label = leftHandLabels.takeLast();
                        delete label;
                    }

                    // Добавить новые
                    for (int i = leftHandLabels.size(); i < leftHand.size(); ++i) {
                        DominoLabel* label = new DominoLabel(this);
                        label->setFixedSize(50, 30);
                        label->setStyleSheet("background: white; border: 1px solid black;");
                        ui->verticalLayoutLeftHand->addWidget(label);
                        leftHandLabels.append(label);
                    }

                    // Обновить верхние костяшки (задняя сторона)
                    for (int i = 0; i < leftHand.size(); ++i) {
                        leftHandLabels[i]->setLineVisible(false); // Скрыть точки
                        leftHandLabels[i]->show();
                    }
                }
            }
            else
            {
                const auto& topHand = game->getNextPlayer()->getHand();
                // Удалить лишние элементы
                while (topHandLabels.size() > topHand.size()) {
                    DominoLabel* label = topHandLabels.takeLast();
                    delete label;
                }

                // Добавить новые
                for (int i = topHandLabels.size(); i < topHand.size(); ++i) {
                    DominoLabel* label = new DominoLabel(this);
                    label->setFixedSize(30, 50);
                    label->setStyleSheet("background: white; border: 1px solid black;");
                    ui->horizontalLayoutTopHand->addWidget(label);
                    topHandLabels.append(label);
                }

                // Обновить верхние костяшки (задняя сторона)
                for (int i = 0; i < topHand.size(); ++i) {
                    topHandLabels[i]->setLineVisible(false); // Скрыть точки
                    topHandLabels[i]->show();
                }
            }
        }
    }
}

void GameWindow::updateHighlightedAreas() {
    // Удалить старую подсветку
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<ClickableRect*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Подсветить допустимые ходы
    const QVector<DominoTile>& board = game->getBoard();
    if (board.isEmpty()) {
        // Подсветка центра для первого хода
        QPoint center(ui->dominoArea->width() / 2, ui->dominoArea->height() / 2);
        drawArrow(center, false, DominoTile(0, 0)); // Заглушка для дубля
        return;
    }

    // Подсветить левый и правый концы доски
    QPoint leftPos = getTilePosition(board.first()); // Используйте свой метод
    QPoint rightPos = getTilePosition(board.last());;

    drawArrow(leftPos, true, board.first());  // Слева от левого конца
    drawArrow(rightPos, false, board.last()); // Справа от правого конца
}

QPoint GameWindow::getTilePosition(const DominoTile& tile) const {
    // поиск позиции по данным домино
    // for (auto item : scene->items()) {
    //     DominoTile* tileItem = dynamic_cast<DominoTile*>(item);
    //     if (tileItem && tileItem->getTile() == tile) {
    //         return item->pos().toPoint();
    //     }
    // }
    return QPoint(0, 0); // Заглушка
}

void GameWindow::onReserveClicked() {
    if (game->getBazaar()->remainingTilesCount() == 0) {
        QMessageBox::information(this, "Резерв пуст", "Нет доступных костяшек.");
        return;
    }
    // Затемнение родительского окна
    m_darkOverlay = new QWidget(this);
    m_darkOverlay->setStyleSheet("background: rgba(0, 0, 0, 150);");
    m_darkOverlay->setGeometry(rect()); // Занимает всю область GameWindow
    m_darkOverlay->setAttribute(Qt::WA_TransparentForMouseEvents); // Игнорировать клики
    m_darkOverlay->show();

    m_bazaarOverlay = new BazaarOverlay(game->getBazaar(), this);
    connect(m_bazaarOverlay, &BazaarOverlay::tileSelected, this, &GameWindow::handleBazaarTileSelected);

    connect(m_bazaarOverlay, &BazaarOverlay::destroyed, this, [this]() {
        m_darkOverlay->deleteLater(); // Безопасное удаление
    });
    m_bazaarOverlay->updateTiles();
    m_bazaarOverlay->show();
}

void GameWindow::handleBazaarTileSelected(const DominoTile& tile) {
    // Добавить домино в руку игрока
    game->getCurrentPlayer()->addTile(tile);
    game->getBazaar()->removeTile(tile);
    // Обновить интерфейс
    updateGameState();
}

void GameWindow::showGameOver() {
    if (isGameOverShown) return; // Защита от повторного вызова
    isGameOverShown = true;
    int winnerIndex = game->determineWinner();
    // Затемнение фона
    m_darkOverlay = new QWidget(this);
    m_darkOverlay->setStyleSheet("background: rgba(0, 0, 0, 150);");
    m_darkOverlay->setGeometry(rect());
    m_darkOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_darkOverlay->show();

    if (winnerIndex != -1) {
        Player* winner = game->getPlayers()[winnerIndex];
        winner->addWin();
    }

    GameOverDialog* dialog = new GameOverDialog(game->getPlayers(), this);
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->adjustSize();
    dialog->move(geometry().center() - dialog->rect().center());
    dialog->show();

    connect(dialog, &GameOverDialog::newRoundRequested, this, [this, dialog]() {
        dialog->close();
        game->blockSignals(false), ui->dominoArea->setFixedSize(669, 349),  ui->scrollDominoArea->setStyleSheet("QScrollArea { border: none; }"),
        ui->dominoArea->setAlignment(Qt::AlignCenter), game->startNewRound(playerNames), isGameOverShown = false, gameOverShown = false, clearBoard();
        updateGameState(); m_darkOverlay->deleteLater(); game->doubleCall = false;
    });

    connect(dialog, &GameOverDialog::exitToMainMenu, this, [this]() {
        // При выходе в меню записываем статистику
        try {
            writeGameStats();
        } catch (const std::exception& e) {
            qCritical() << "Ошибка записи статистики:" << e.what();
        }
        onHomeClicked();
    });
}


void GameWindow::onPlayerChanged(int index) {
    const auto& players = game->getPlayers();
    if (!players[index]->isBot() && !isNetworkGame) {
        showTurnOverlay(players[index]->getName());
    }
    else {
        m_turnOverlay->hide();
        if (!isNetworkGame && players[index]->isBot()) {
            QTimer::singleShot(1000, this, [this]() {
                processMove(DominoTile(-1, -1));
            });
        }
    }
    updateGameState();
}

void GameWindow::showTurnOverlay(const QString& name) {
    if (!game->getCurrentPlayer()->isBot() && !game->getNextPlayer()->isBot()) {
        m_turnLabel->setText(QString("Ваш ход, %1\nНажмите escape").arg(name));
        m_turnOverlay->setGeometry(rect());
        m_turnOverlay->raise();
        m_turnOverlay->show();
        isHandleNoValidMoves = false;
        enableSpaceKey();
    } else {
        m_turnOverlay->hide();
        disableSpaceKey();
    }
}

// Обработка нажатия esc
void GameWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        if (m_spaceKeyEnabled && m_turnOverlay->isVisible()) {
            m_turnOverlay->hide(); // Только скрытие оверлея
            event->accept(); // Запрет дальнейшей обработки esc
        }
        return; // Выход без передачи события родителю
    }
    QWidget::keyPressEvent(event);
}

void GameWindow::clearHighlights() {
    // Удалить все элементы подсветки со сцены
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<ClickableRect*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }
    for (DominoLabel* label : bottomHandLabels) {
        label->setHighlighted(false);
    }
}

void GameWindow::writeGameStats() {
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/stats.json";
    QFile file(filePath);
    QJsonArray statsArray;

    // Чтение существующих данных
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        statsArray = doc.array();
        file.close();
    }

    // Создание новой записи для каждого игрока
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString gameId = QString::number(currentDateTime.toSecsSinceEpoch());

    for (Player* player : game->getPlayers()) {
        if (!player) continue; // Проверка на nullptr

        QJsonObject entry;
        entry["game_id"] = gameId;
        entry["name"] = player->getName().isEmpty() ? "Unknown" : player->getName(); // Защита от пустого имени
        entry["wins"] = player->getWins();
        entry["points"] = player->getScore();
        entry["rounds"] = game->getCurrentRound();
        entry["players"] = game->getPlayers().size();
        entry["date"] = currentDateTime.toString("yyyy-MM-dd");

        // Формируем sortKey с учетом всех критериев
        entry["sortKey"] = QString("%1-%2-%3")
                               .arg(entry["points"].toInt(), 10, 10, QChar('0'))
                               .arg(-entry["rounds"].toInt(), 10, 10, QChar('0'))
                               .arg(currentDateTime.toString("yyyyMMddHHmmss")); // Уникальный ключ для сортировки

        statsArray.append(entry);
        qDebug() << "Запись игрока:" << entry["name"] << entry["points"] << entry["rounds"];
    }

    // Сортировка через временный список объектов
    QList<QJsonObject> entries;
    for (const QJsonValue& val : statsArray) {
        entries.append(val.toObject());
    }

    std::sort(entries.begin(), entries.end(), [](const QJsonObject& a, const QJsonObject& b) {
        return a["sortKey"].toString() < b["sortKey"].toString();
    });

    // Перезаписываем массив
    statsArray = QJsonArray();
    for (const QJsonObject& obj : entries) {
        statsArray.append(obj);
    }

    // Сохранение в файл
    QFile outFile(filePath);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        outFile.write(QJsonDocument(statsArray).toJson());
        outFile.close();
    } else {
        qDebug() << "Ошибка записи в файл:" << outFile.errorString();
    }
}

void GameWindow::updateUI() {
    bool canMove = game->currentPlayerCanMove();
    ui->pushButton_skip->setEnabled(!game->getCurrentPlayer()->isBot() && !canMove); // Активируем кнопку, если ходов нет
}

void GameWindow::on_pushButton_skip_clicked() {
    if (!game->currentPlayerCanMove() && game->getBazaar()->isEmpty() && !game->checkForBlockedGame()) {
        int nextIndex = (game->getCurrentPlayerIndex() + 1) % game->getPlayers().size();
        game->setCurrentPlayerIndex(nextIndex);
        emit game->playerChanged(nextIndex);     // Переход к следующему игроку
        onPlayerChanged(nextIndex);
        updateGameState();      // Обновление состояния игры
        updateHandDisplay();
    }
}

void GameWindow::highlightValidTiles() {
    // Проверка на необходимость действий при отсутствии ходов
    if (!game->currentPlayerCanMove() && !isHandleNoValidMoves) {
        isHandleNoValidMoves = true;
        handleNoValidMoves();
    }
    QVector<int> ends = game->getBoardEnds();
    for (DominoLabel* label : bottomHandLabels) {
        DominoTile tile = getTileFromLabel(label);
        bool isValid = false;

        if (ends.isEmpty()) {
            // Проверка первого хода
            bool isDouble = tile.isDouble();
            isValid = isDouble && (tile.getLeftValue() != 0);
        } else {
            isValid = tile.getLeftValue() == ends.first() ||
                      tile.getLeftValue() == ends.last() ||
                      tile.getRightValue() == ends.first() ||
                      tile.getRightValue() == ends.last();
        }

        label->setHighlighted(isValid, Qt::green);
    }
}
void GameWindow::handleNoValidMoves() {
    if (!game->getBazaar()->isEmpty()) {
        QMessageBox::information(this, "Подсказка",
                                 "Нет подходящих ходов. Возьмите костяшку из базара!");
    } else {
        QMessageBox::information(this, "Подсказка",
                                 "Базар пуст. Нажмите 'Пропустить ход'");
        ui->pushButton_skip->setEnabled(true);
    }
}

void GameWindow::updateReserveLabel(bool showReserve) {
    reserveButton->setText("Резерв: " + QString::number(game->getBazaar()->remainingTilesCount()));
    if (!showReserve) {
        reserveButton->setText("Резерв ");
    }
}

void GameWindow::showFinalGameOver() {
    if (isGameOverShown) return;
    isGameOverShown = true;

    // Определяем победителя
    QVector<int> totalScores = game->getScores();
    int winnerIndex = 0;
    int maxScore = 0;
    bool isDraw = false;

    for (int i = 0; i < totalScores.size(); ++i) {
        if (totalScores[i] > maxScore) {
            maxScore = totalScores[i];
            winnerIndex = i;
            isDraw = false;
        } else if (totalScores[i] == maxScore) {
            isDraw = true;
        }
    }

    // Затемнение фона
    m_darkOverlay = new QWidget(this);
    m_darkOverlay->setStyleSheet("background: rgba(0, 0, 0, 150);");
    m_darkOverlay->setGeometry(rect());
    m_darkOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_darkOverlay->show();

    Player* winner = game->getPlayers()[winnerIndex];

    // Запись статистики
    try {
        if (!isDraw) {
            winner->addWin();
        }
        writeGameStats();
    } catch (const std::exception& e) {
        qCritical() << "Ошибка записи статистики:" << e.what();
    }

    // Создаем финальное окно победы
    FinalGameOverDialog* dialog = new FinalGameOverDialog(game->getPlayers(), game->getScores(), game->getMaxScore(), isDraw,this);

    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->adjustSize();
    dialog->move(geometry().center() - dialog->rect().center());
    dialog->show();

    connect(dialog, &FinalGameOverDialog::exitToMainMenu, this, &GameWindow::onHomeClicked);
}

void GameWindow::updateFromJson(const QJsonObject& state)
{
    game->deserializeFromJson(state);
    determinePlayerPositions(state);
    // Сбрасываем выделение
    clearSelection();

    // Обновляем UI
    updateGameState();
    updateBoard();
    updateHandDisplay();
}
void GameWindow::setClient(Client* server) {
    client = server;
    this->client = client;

    connect(client, &Client::gameStarted, this, &GameWindow::updateFromJson);
    connect(client, &Client::newChatMessage, this, [this](const QString& sender, const QString& message) {
        chatManager->sendMessage(sender, message);
    });
    connect(client, &Client::gameStateReceived, this, &GameWindow::updateFromJson);
}

void GameWindow::determinePlayerPositions(const QJsonObject& state)
{
    QJsonArray playersArray = state["players"].toArray();
    QStringList players;
    for (const QJsonValue& player : playersArray) {
        players.append(player.toObject()["name"].toString());
    }

    // Находим индекс текущего игрока
    int ourIndex = players.indexOf(clientPlayerName);
    if (ourIndex == -1) return;

    // Определяем позиции для отображения
    playerPositions.clear();
    for (int i = 0; i < players.size(); i++) {
        int relativePos = (i - ourIndex + players.size()) % players.size();
        playerPositions[players[i]] = relativePos;
    }
}

void GameWindow::updateHandForPosition(Player* player, int position, bool isOpen)
{
    QVector<DominoLabel*>* labels = nullptr;
    QLayout* layout = nullptr;
    QSize size;

    switch (position) {
    case 0: // Наш игрок (низ)
        layout = ui->horizontalLayoutBottomHand;
        labels = &bottomHandLabels;
        size = QSize(40, 80);
        break;
    case 1: // Правый игрок
        layout = ui->verticalLayoutRightHand;
        labels = &rightHandLabels;
        size = QSize(30, 50);
        break;
    case 2: // Верхний игрок
        layout = ui->horizontalLayoutTopHand;
        labels = &topHandLabels;
        size = QSize(30, 50);
        break;
    case 3: // Левый игрок
        layout = ui->verticalLayoutLeftHand;
        labels = &leftHandLabels;
        size = QSize(30, 50);
        break;
    }

    // Очищаем текущие элементы
    while (!labels->isEmpty()) {
        DominoLabel* label = labels->takeLast();
        layout->removeWidget(label);
        delete label;
    }

    // Добавляем костяшки
    for (const DominoTile& tile : player->getHand()) {
        DominoLabel* label = new DominoLabel(this);
        label->setFixedSize(size);

        if (isOpen) {
            // Свои костяшки - открытые
            label->setDots(tile.getLeftValue(), tile.getRightValue(), true, true);
            connect(label, &DominoLabel::clicked, this, [this, label]() {
                handleDominoClick(label);
            });
        } else {
            // Чужие костяшки - закрытые
            label->setLineVisible(false);
        }

        layout->addWidget(label);
        labels->append(label);
    }
}

void GameWindow::loadGameState(const QJsonObject& state)
{
    // 1. Десериализация состояния игры
    if (existingGame) {
        existingGame->deserializeFromJson(state);
    } else {
        qWarning() << "No game instance available for deserialization";
        return;
    }

    // 2. Обновление интерфейса
    // a) Доска
    QJsonArray boardArray = state["board"].toArray();
    // Очистка текущей доски
    QGraphicsScene* scene = ui->dominoArea->scene();
    if (!scene) {
        scene = new QGraphicsScene(this);
        ui->dominoArea->setScene(scene);
    } else {
        scene->clear();
    }

    const QVector<DominoTile>& board = existingGame->getBoard();

    // Рассчитываем начальную позицию (центр экрана)
    int currentX = ui->dominoArea->width() / 2 - 30;
    int centerY = ui->dominoArea->height() / 2;
    int yForDouble = centerY - 40;    // Для дублей выше центра
    int yForNonDouble = centerY + 10; // Для обычных костяшек ниже центра

    // Создаем графические элементы для каждой костяшки
    for (const DominoTile& tile : board) {
        bool isDouble = (tile.getLeftValue() == tile.getRightValue());
        int width = isDouble ? 40 : 80;
        int height = isDouble ? 80 : 40;

        DominoLabel* label = new DominoLabel();
        label->setDots(tile.getLeftValue(), tile.getRightValue(), isDouble, true);
        label->setFixedSize(width, height);

        QGraphicsProxyWidget* proxy = scene->addWidget(label);
        proxy->setPos(currentX, isDouble ? yForDouble : yForNonDouble);

        currentX += width + 5; // Перемещаем позицию для следующей костяшки
    }

    // b) Руки игроков
    updateHandDisplay();

    // c) Базар
    QJsonArray bazaarArray = state["bazaar_tiles"].toArray();
    // Очистка текущего базара
    QLayoutItem* item1;
    while ((item1 = ui->labelReserveLayout->takeAt(0))) {
        delete item1->widget();
        delete item1;
    }

    // Добавление костяшек в базар
    for (const QJsonValue& tileValue : bazaarArray) {
        QJsonObject tileObj = tileValue.toObject();

        DominoLabel* tileLabel = new DominoLabel(this);
        tileLabel->setFixedSize(30, 50);
        tileLabel->setLineVisible(false); // Тыльная сторона
        tileLabel->setStyleSheet("background: white; border: 1px solid black;");

        ui->labelReserveLayout->addWidget(tileLabel);
    }

    // Счет игроков
    QJsonArray scoresArray = state["player_scores"].toArray();
    QJsonArray playersArray = state["players"].toArray();

    // Сначала создаем список имен игроков
    QStringList playerNames;
    for (int i = 0; i < playersArray.size(); ++i) {
        playerNames.append(playersArray[i].toObject()["name"].toString());
    }

    // Обновляем счет
    for (int i = 0; i < scoresArray.size(); ++i) {
        int score = scoresArray[i].toInt();
        QString playerName = playerNames[i];

        // Проверяем, есть ли позиция для этого игрока
        if (!playerPositions.contains(playerName)) continue;

        int position = playerPositions[playerName];
        switch (position) {
        case 0: // Bottom
            ui->labelScoreBottom->setText(QString::number(score));
            break;
        case 1: // Right
            ui->labelScoreRight->setText(QString::number(score));
            break;
        case 2: // Top
            ui->labelScoreTop->setText(QString::number(score));
            break;
        case 3: // Left
            ui->labelScoreLeft->setText(QString::number(score));
            break;
        }
    }

    // Текущий игрок
    QString currentPlayer = state["current_player"].toString();
    // Сброс подсветки для всех игроков
    ui->labelNameBottom->setStyleSheet("");
    ui->labelNameRight->setStyleSheet("");
    ui->labelNameTop->setStyleSheet("");
    ui->labelNameLeft->setStyleSheet("");

    // Подсвечиваем текущего игрока по имени
    if (playerPositions.contains(currentPlayer)) {
        int position = playerPositions[currentPlayer];
        switch (position) {
        case 0: // Bottom
            ui->labelNameBottom->setStyleSheet("background-color: black; font-weight: bold;");
            break;
        case 1: // Right
            ui->labelNameRight->setStyleSheet("background-color: black; font-weight: bold;");
            break;
        case 2: // Top
            ui->labelNameTop->setStyleSheet("background-color: black; font-weight: bold;");
            break;
        case 3: // Left
            ui->labelNameLeft->setStyleSheet("background-color: black; font-weight: bold;");
            break;
        }
    }

    // Обновление имен игроков
    for (int i = 0; i < playerNames.size(); ++i) {
        QString playerName = playerNames[i];
        if (!playerPositions.contains(playerName)) continue;

        int position = playerPositions[playerName];
        switch (position) {
        case 0: // Bottom
            ui->labelNameBottom->setText(playerName);
            break;
        case 1: // Right
            ui->labelNameRight->setText(playerName);
            break;
        case 2: // Top
            ui->labelNameTop->setText(playerName);
            break;
        case 3: // Left
            ui->labelNameLeft->setText(playerName);
            break;
        }
    }

    // Перерисовка интерфейса
    repaint();
}

void GameWindow::handleNetworkGameOver(const QVector<QString>& playerNames, const QVector<int>& playerScores, int maxScore, const QString& winner, bool isDraw)  {
    QVector<Player*> players;
    for (int i = 0; i < playerNames.size(); i++) {
        Player* p = new HumanPlayer(playerNames[i]);
        p->setScore(playerScores[i]);
        players.append(p);
    }

    showFinalGameOverNetwork(players, playerScores, maxScore, isDraw, winner);

    // Очищаем временные объекты после использования
    QTimer::singleShot(0, this, [players]() {
        qDeleteAll(players);
    });
}

void GameWindow::showFinalGameOverNetwork(const QVector<Player*>& players, const QVector<int>& scores, int maxScore, bool isDraw, const QString& winnerName) {
    if (isGameOverShown) return;
    isGameOverShown = true;

    // Затемнение фона
    if (!m_darkOverlay) {
        m_darkOverlay = new QWidget(this);
        m_darkOverlay->setStyleSheet("background: rgba(0, 0, 0, 150);");
        m_darkOverlay->setGeometry(rect());
        m_darkOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
    m_darkOverlay->show();

    try {
        if (!isDraw) {
            // Находим настоящего победителя в локальной игре
            Player* winner = nullptr;
            for (Player* p : players) {
                if (p->getName() == winnerName) {
                    winner = p;
                    break;
                }
            }
            if (winner) winner->addWin();
            writeGameStats();
        }
    } catch (const std::exception& e) {
        qCritical() << "Ошибка записи статистики:" << e.what();
    }


    // Создаем финальное окно победы
    FinalGameOverDialog* dialog = new FinalGameOverDialog(players, scores, maxScore, isDraw, this);

    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->adjustSize();
    dialog->move(geometry().center() - dialog->rect().center());
    dialog->show();

    connect(dialog, &FinalGameOverDialog::exitToMainMenu, this, &GameWindow::onHomeClicked);
}
