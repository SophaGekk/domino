#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include "bazaar.h"
#include "player.h"
#include "dominolabel.h"
#include "bot_player.h"
#include "clickable_rect.h"
#include <QGraphicsProxyWidget>
#include "clickablelabel.h"
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
#include "humanplayer.h"

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
    ui->labelReserveLayout->addWidget(reserveButton);
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


    m_currentPlayerLabel = new QLabel(this);
    m_currentPlayerLabel->setAlignment(Qt::AlignCenter);
    m_currentPlayerLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; background: rgba(255,255,255,0.7); border-radius: 10px; padding: 5px;");
    m_currentPlayerLabel->setGeometry(width()/2 - 150, 10, 300, 30);
    m_currentPlayerLabel->hide();

    m_turnHintTimer = new QTimer(this);
    m_turnHintTimer->setSingleShot(true);
    connect(m_turnHintTimer, &QTimer::timeout, this, [this]() {
        m_currentPlayerLabel->hide();
    });

    updateGameState();
    setupHands();
    updateUI();
}


void GameWindow::updateGameState() {
    if (game->isGameOver()) {
        int winnerIndex = game->determineWinner();
        Player* winner = game->getPlayers()[winnerIndex];

        // Блокируем дальнейшие действия для ботов
        if (!isNetworkGame) {
            game->blockSignals(true);
        }

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

    game->updateNextPlayerIndex();
    const auto& players = game->getPlayers();
    if(players.size() >= 3) {
        game->updateThirdPlayerIndex();
        if(players.size() == 4) {
            game->updateFourthPlayerIndex();
        }
    }

    // Обновление информации об игроках
    if (isNetworkGame) {
        int localIndex = -1;
        for (int i = 0; i < players.size(); ++i) {
            if (players[i]->getName() == clientPlayerName) {
                localIndex = i;
                break;
            }
        }

        if (localIndex == -1) {
            qWarning() << "Local player not found!";
            return;
        }

        // внизу
        Player* localPlayer = players[localIndex];
        ui->labelNameBottom->setText(localPlayer->getName());
        ui->labelScoreBottom->setText(QString::number(localPlayer->getScore()));
        ui->avatarBottom->setPixmap(localPlayer->getAvatar());

        // Распределение остальных игроков
        if (players.size() >= 3) {
            // Правый игрок: следующий после локального
            int rightIndex = (localIndex + 1) % players.size();
            Player* rightPlayer = players[rightIndex];
            ui->labelNameRight->setText(rightPlayer->getName());
            ui->labelScoreRight->setText(QString::number(rightPlayer->getScore()));
            // ui->avatarRight->setPixmap(rightPlayer->getAvatar());

            // Верхний игрок: через одного от локального
            int topIndex = (localIndex + 2) % players.size();
            Player* topPlayer = players[topIndex];
            ui->labelNameTop->setText(topPlayer->getName());
            ui->labelScoreTop->setText(QString::number(topPlayer->getScore()));
            ui->avatarTop->setPixmap(topPlayer->getAvatar());

            // Левый игрок (только для 4 игроков)
            if (players.size() == 4) {
                int leftIndex = (localIndex + 3) % players.size();
                Player* leftPlayer = players[leftIndex];
                ui->labelNameLeft->setText(leftPlayer->getName());
                ui->labelScoreLeft->setText(QString::number(leftPlayer->getScore()));
                // ui->avatarLeft->setPixmap(leftPlayer->getAvatar());
            }
        }
        else {
            int topIndex = (localIndex + 1) % players.size();
            Player* topPlayer = players[topIndex];
            ui->labelNameTop->setText(topPlayer->getName());
            ui->labelScoreTop->setText(QString::number(topPlayer->getScore()));
            ui->avatarTop->setPixmap(topPlayer->getAvatar());
        }
    }
    else {
        ui->labelNameBottom->setText(game->getCurrentPlayer()->getName());
        ui->labelScoreBottom->setText(QString::number(game->getCurrentPlayer()->getScore()));
        ui->avatarBottom->setPixmap(game->getCurrentPlayer()->getAvatar()); // Установка аватарки

        if(players.size() >= 3)
        {
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
    }


    // Проверка, является ли ход текущего клиента
    bool isOurTurn = false;
    if (isNetworkGame) {
        QString currentPlayerName = game->getCurrentPlayer()->getName();

        // Отображение информации о текущем игроке
        m_currentPlayerLabel->setText("Сейчас ходит: " + currentPlayerName);
        m_currentPlayerLabel->show();
        m_turnHintTimer->start(3000); // Скрыть через 3 секунды
        isOurTurn = (currentPlayerName == clientPlayerName);

        for (DominoLabel* label : bottomHandLabels) {
            label->setEnabled(isOurTurn);
        }
        reserveButton->setEnabled(isOurTurn);
        // Если это наш ход и мы в сетевой игре, показать подсказку
        if (isOurTurn) {
            QMessageBox::information(this, "Ваш ход", "Сейчас ваша очередь сделать ход!");
        }
    }


    updateHandDisplay();
    // Подсветка допустимых ходов
    if (isHighlightEnabled && !game->getCurrentPlayer()->isBot() && !game->isGameOver()) {
        if(!isNetworkGame || (isNetworkGame && isOurTurn))
        {
           highlightValidTiles();
        }

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
    if (isNetworkGame && !game->isGameFinished()) {
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(
            this, tr("Подтверждение выхода"),tr("Вы уверены, что хотите покинуть игру? Текущая сессия будет завершена."), QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

        if (confirm == QMessageBox::No) {
            return; // Отмена выхода
        }
        client->leaveSession();
        if (!this->isHost)
        {
            // При выходе в меню записываем статистику
            try {
                writeGameStats();
                qDebug() << "writeGameStats запись";

            } catch (const std::exception& e) {
                qCritical() << "Ошибка записи статистики:" << e.what();
            }
        }
    }
    if (isNetworkGame && client) {
        client->disconnectFromServer();
        client->deleteLater();
        client = nullptr;
    }
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
        // Получаем список игроков
        const auto& players = game->getPlayers();
        int localIndex = -1;

        // Находим индекс локального игрока
        for (int i = 0; i < players.size(); ++i) {
            if (players[i]->getName() == clientPlayerName) {
                localIndex = i;
                break;
            }
        }

        if (localIndex == -1) return; // Локальный игрок не найден

        // Обновляем руки для каждой позиции
        updateHandForPosition(players[localIndex], 0, true); // Наш игрок (низ)

        if (players.size() >= 3) {
            int rightIndex = (localIndex + 1) % players.size();
            updateHandForPosition(players[rightIndex], 1, false); // Правый игрок

            int topIndex = (localIndex + 2) % players.size();
            updateHandForPosition(players[topIndex], 2, false); // Верхний игрок

            if (players.size() == 4) {
                int leftIndex = (localIndex + 3) % players.size();
                updateHandForPosition(players[leftIndex], 3, false); // Левый игрок
            }
        }
        else
        {
            int topIndex = (localIndex + 1) % players.size();
            updateHandForPosition(players[topIndex], 2, false); // Верхний игрок
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
                    const auto& leftHand = game->getFourthPlayer()->getHand();;
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
    if (isNetworkGame) {
        client->sendBazaarTileRequest(tile);

        // Закрываем оверлей
        if (m_bazaarOverlay) {
            m_bazaarOverlay->close();
            m_bazaarOverlay->deleteLater();
            m_bazaarOverlay = nullptr;
        }
    } else {
        game->getCurrentPlayer()->addTile(tile);
        game->getBazaar()->removeTile(tile);
        updateGameState();
    }
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

    if(isNetworkGame)
    {
        connect(dialog, &GameOverDialog::newRoundRequested, this, [this, dialog]() {
            dialog->close();

            game->blockSignals(false), ui->dominoArea->setFixedSize(669, 349),  ui->scrollDominoArea->setStyleSheet("QScrollArea { border: none; }"),
            ui->dominoArea->setAlignment(Qt::AlignCenter);

            // Отправляем запрос на новый раунд
            if (client && this->isHost) {
                client->sendNewRoundRequest();
                updateGameState();gameOverShown = false;isGameOverShown = false, clearBoard(); m_darkOverlay->deleteLater(); game->doubleCall = false;
            }
            else if(client)
            {
                onNetworkError("Только Хост может начать новый раунд!");
                updateGameState();gameOverShown = false;isGameOverShown = false, clearBoard(); m_darkOverlay->deleteLater(); game->doubleCall = false;
            }
        });
    }
    else
    {
        connect(dialog, &GameOverDialog::newRoundRequested, this, [this, dialog]() {
            dialog->close();
            game->blockSignals(false), ui->dominoArea->setFixedSize(669, 349),  ui->scrollDominoArea->setStyleSheet("QScrollArea { border: none; }"),
            ui->dominoArea->setAlignment(Qt::AlignCenter), game->startNewRound(playerNames), isGameOverShown = false, gameOverShown = false, clearBoard();
            updateGameState(); m_darkOverlay->deleteLater(); game->doubleCall = false;
        });
    }

    connect(dialog, &GameOverDialog::exitToMainMenu, this, [this, dialog]() {
        if ((client && this->isHost) || !isNetworkGame)
        {
            // При выходе в меню записываем статистику
            try {
                writeGameStats();
                qDebug() << "writeGameStats запись";

            } catch (const std::exception& e) {
                qCritical() << "Ошибка записи статистики:" << e.what();
            }
        }
        dialog->close();
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

    if (event->key() == Qt::Key_H) {
        showTurnHint();
        highlightValidTiles();
        event->accept();
        return;
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
    qint64 timestamp = currentDateTime.toSecsSinceEpoch(); // Для точной сортировки

    for (Player* player : game->getPlayers()) {
        if (!player) continue; // Проверка на nullptr

        QJsonObject entry;
        entry["game_id"] = QString::number(timestamp);
        entry["name"] = player->getName().isEmpty() ? "Unknown" : player->getName();
        entry["wins"] = player->getWins();
        entry["points"] = player->getScore();
        entry["rounds"] = game->getCurrentRound();
        entry["players"] = game->getPlayers().size();
        entry["date"] = currentDateTime.toString("yyyy-MM-dd");
        entry["timestamp"] = timestamp; // Добавляем метку времени

        statsArray.append(entry);
        qDebug() << "Запись игрока:" << entry["name"] << entry["points"] << entry["rounds"];
    }

    // Сортировка через временный список объектов
    QList<QJsonObject> entries;
    for (const QJsonValue& val : statsArray) {
        entries.append(val.toObject());
    }

    std::sort(entries.begin(), entries.end(), [](const QJsonObject& a, const QJsonObject& b) {
        // 1. очки (по убыванию)
        int pointsA = a["points"].toInt();
        int pointsB = b["points"].toInt();
        if (pointsA != pointsB) {
            return pointsA > pointsB;
        }

        // 2. Дата/время (по убыванию - свежие выше)
        qint64 timeA = a["timestamp"].toVariant().toLongLong();
        qint64 timeB = b["timestamp"].toVariant().toLongLong();
        if (timeA != timeB) {
            return timeA > timeB;
        }

        // 3. Имя игрока (по возрастанию A-Z)
        QString nameA = a["name"].toString();
        QString nameB = b["name"].toString();
        if (nameA != nameB) {
            return nameA < nameB;
        }

        // 4. Победы (по убыванию)
        int winsA = a["wins"].toInt();
        int winsB = b["wins"].toInt();
        if (winsA != winsB) {
            return winsA > winsB;
        }

        // 5. Раунды (по возрастанию - меньше лучше)
        int roundsA = a["rounds"].toInt();
        int roundsB = b["rounds"].toInt();
        return roundsA < roundsB;
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
    if (isNetworkGame) {
        QMessageBox::information(this, "Ход пропущен",  QString("%1 пропускает ход").arg(game->getCurrentPlayer()->getName()));
        client->sendSkipRequest();
    } else {
        if (!game->currentPlayerCanMove() && game->getBazaar()->isEmpty() && !game->checkForBlockedGame()) {
            int nextIndex = (game->getCurrentPlayerIndex() + 1) % game->getPlayers().size();
            game->setCurrentPlayerIndex(nextIndex);
            emit game->playerChanged(nextIndex);     // Переход к следующему игроку
            onPlayerChanged(nextIndex);
            updateGameState();      // Обновление состояния игры
            updateHandDisplay();
        }
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
                                 "Нет подходящих ходов. Возьмите костяшку из Резерва!");
    } else {
        QMessageBox::information(this, "Подсказка",
                                 "Резерв пуст. Нажмите 'Пропустить ход'");
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
    if ((client && this->isHost) || !isNetworkGame)
    {
        // Запись статистики
        try {
            if (!isDraw) {
                winner->addWin();
            }
            writeGameStats();
        } catch (const std::exception& e) {
            qCritical() << "Ошибка записи статистики:" << e.what();
        }
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
    updateBoard();
    updateHandDisplay();
}

void GameWindow::setClient(Client* client) {
    this->client = client;

    connect(client, &Client::gameStarted, this, &GameWindow::updateFromJson);
    connect(client, &Client::newChatMessage, this, [this](const QString& sender, const QString& message) {
        chatManager->sendMessage(sender, message);
    });
    connect(client, &Client::gameStateReceived, this, &GameWindow::updateFromJson);
    connect(client, &Client::gameOver, this, &GameWindow::showFinalGameOver);
    connect(client, &Client::roundOver, this, &GameWindow::showGameOver);
    connect(client, &Client::errorOccurred, this, &GameWindow::onNetworkError);
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

void GameWindow::updateHandForPosition(Player* player, int position, bool isOpen) {
    if (!player) return;

    QVector<DominoLabel*>* labels = nullptr;
    QLayout* layout = nullptr;
    QSize size;

    switch (position) {
    case 0: // Наш игрок (низ)
        labels = &bottomHandLabels;
        layout = ui->horizontalLayoutBottomHand;
        size = QSize(40, 80);
        break;
    case 1: // Правый игрок
        labels = &rightHandLabels;
        layout = ui->verticalLayoutRightHand;
        size = QSize(50, 30);
        break;
    case 2: // Верхний игрок
        labels = &topHandLabels;
        layout = ui->horizontalLayoutTopHand;
        size = QSize(30, 50);
        break;
    case 3: // Левый игрок
        labels = &leftHandLabels;
        layout = ui->verticalLayoutLeftHand;
        size = QSize(50, 30);
        break;
    default:
        return;
    }

    const auto& hand = player->getHand();

    // Удалить лишние элементы
    while (labels->size() > hand.size()) {
        DominoLabel* label = labels->takeLast();
        layout->removeWidget(label);
        delete label;
    }

    // Добавить новые элементы
    for (int i = labels->size(); i < hand.size(); ++i) {
        DominoLabel* label = new DominoLabel(this);
        label->setFixedSize(size);
        label->setStyleSheet("background: white; border: 1px solid black;");

        if (isOpen) {
            // Для своих костяшек добавляем обработчик клика
            connect(label, &DominoLabel::clicked, this, [this, label]() {
                handleDominoClick(label);
            });
        }

        layout->addWidget(label);
        labels->append(label);
    }

    // Обновить значения для всех костяшек
    for (int i = 0; i < hand.size(); ++i) {
        DominoTile tile = hand[i];
        if (isOpen) {
            // Свои костяшки - открытые
            labels->at(i)->setDots(tile.getLeftValue(), tile.getRightValue(), true, true);
        } else {
            // Чужие костяшки - закрытые
            labels->at(i)->setLineVisible(false);
        }
        labels->at(i)->show();
    }
}

void GameWindow::setHost(bool isHost) {
    this->isHost = isHost;
}

void GameWindow::onNetworkError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void GameWindow::showTurnHint() {
    QString hint;

    if (game->getBoard().isEmpty()) {
        hint = "Первый ход: нужно выложить дубль или самую 'тяжелую' костяшку";
    } else {
        QVector<int> ends = game->getBoardEnds();
        hint = QString("Текущие концы доски: %1 и %2\n"
                       "Вы можете ходить костяшкой, которая совпадает с одним из концов")
                   .arg(ends.first()).arg(ends.last());
    }

    QMessageBox::information(this, "Подсказка", hint);
}
