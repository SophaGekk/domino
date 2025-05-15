#include "domino_game.h"
#include "bazaar.h"
#include "player.h"
#include "domino_tile.h"
#include "humanplayer.h"
#include <QDateTime>
#include <QDataStream>
#include "bot_player.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


// Конструктор
DominoGame::DominoGame(int playerCount, int BotCount, QObject* parent)
    : QObject(parent), playerCount(playerCount), BotCount(BotCount), currentPlayerIndex(0), gameEnd(false) {
    bazaar = new Bazaar();
    initializePlayers();

}

// Реализация методов доступа
const QVector<Player*>& DominoGame::getPlayers() const {
    return players;
}

const QVector<DominoTile>& DominoGame::getBoard() const {
    return board;
}

QVector<DominoTile>& DominoGame::getBoard()  {
    return board;
}

const Bazaar* DominoGame::getBazaarConst() const {
    return bazaar;
}

Bazaar* DominoGame::getBazaar()  {
    return bazaar;
}
int DominoGame::getCurrentPlayerIndex() const {
    return currentPlayerIndex;
}
int DominoGame::getNextPlayerIndex() const {
    return (currentPlayerIndex + 1) % players.size();;
}

// Инициализация игроков
void DominoGame::initializePlayers() {
    players.clear();

    // players.append(new HumanPlayer(QString("Player %1").arg(0)));
    // players.append(new HumanPlayer(QString("Player %1").arg(1)));
    for (int i = 0; i < playerCount - BotCount; ++i) {
        players.append(new HumanPlayer(QString("Player %1").arg(i + 1)));
    }
    for (int i = 0; i < BotCount; ++i) {
        players.append(new BotPlayer(QString("Bot %1").arg(i + 1)));
    }

}
// Раздача костяшек
void DominoGame::dealTiles() {
    bazaar->initialize();

    int tilesPerPlayer = (playerCount == 2) ? 7 : 5;

    for (Player* player : players) {
        QVector<DominoTile> hand;
        for (int i = 0; i < tilesPerPlayer; ++i) {
            if (!bazaar->isEmpty()) {
                hand.append(bazaar->drawTile());
            }
        }
        player->setHand(hand);
    }
}

// Определение первого игрока
void DominoGame::determineFirstPlayer() {
    hasDouble = false;
    int maxDouble = -1;
    int firstPlayerIndex = 0;

    // Ищем игрока с дублем 6-6
    for (int i = 0; i < players.size(); ++i) {
        for (const DominoTile& tile : players[i]->getHand()) {
            if (tile.getLeftValue() == tile.getRightValue() && tile.getLeftValue() > maxDouble && (tile.getRightValue() != 0)) {
                hasDouble = true;
                maxDouble = tile.getLeftValue();
                firstPlayerIndex = i;
            }
        }
    }

    if(!hasDouble)
    {
        // Если нет дублей, ищем костяшку с максимальной суммой
        int maxSum = -1;
        for (int i = 0; i < players.size(); ++i) {
            for (const DominoTile& tile : players[i]->getHand()) {
                int sum = tile.getTotalValue();
                if (sum > maxSum) {
                    maxSum = sum;
                    firstPlayerIndex = i;
                }
            }
        }
    }
    currentPlayerIndex = firstPlayerIndex;
    updateNextPlayerIndex();
    updateThirdPlayerIndex();
    updateFourthPlayerIndex();
    const auto& players = getPlayers();

    if (players[currentPlayerIndex]->isBot()) {
        emit playerChanged(currentPlayerIndex); // Уведомляем об изменении игрока
    }

}

void DominoGame::updateNextPlayerIndex() {
    NextPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

// Проверка окончания игры
bool DominoGame::isGameOver() const {
    if (gameEnd) return true;

    // Проверяем, есть ли игрок без костяшек
    for (Player* player : players) {
        if (player->getHandSize() == 0) {
            return true;
        }
    }

    // Проверяем на "рыбу" (блокировку)
    return checkForBlockedGame();
}

bool DominoGame::checkForBlockedGame() const {
    if (board.isEmpty() || !bazaar->isEmpty()) return false;

    int leftEnd = board.first().getLeftValue();
    int rightEnd = board.last().getRightValue();

    // Проверяем, есть ли у кого-то подходящие костяшки
    for (Player* player : players) {
        if (player->hasMatchingTile(leftEnd) || player->hasMatchingTile(rightEnd)) {
            return false;
        }
    }

    return true;
}

void DominoGame::startNewGame() {
    setGameId(QDateTime::currentDateTime().toString(Qt::ISODate));
    // Очищаем предыдущее состояние
    board.clear();
    bazaar->initialize();

    // Инициализируем игроков
    initializePlayers();

    // Раздаем костяшки
    dealTiles();

    // Определяем, кто ходит первым
    determineFirstPlayer();
    qDebug() << "game->getCurrentPlayer()" << getCurrentPlayerIndex();
    qDebug() << "game->getNextPlayer()" << getNextPlayerIndex();
    gameEnd = false;
    emit gameStarted();
}


void DominoGame::checkForFish() {
    if (gameEnd || !checkForBlockedGame()) return;

    gameEnd = true;
    calculateScores();
    int winnerIndex = determineWinner();
    emit gameEnded(winnerIndex, getScores());
}

void DominoGame::calculateScores() {
    for (Player* player : players) {
        int score = 0;
        for (const DominoTile& tile : player->getHand()) {
            score += tile.getTotalValue();
        }
        player->setScore(score);
    }
}

QVector<int> DominoGame::getScores() const {
    QVector<int> scores;
    for (const Player* player : players) {
        scores.append(player->getScore());
    }
    return scores;
}

int DominoGame::determineWinner() const {
    // Проверяем, есть ли игрок с пустой рукой
    for (int i = 0; i < players.size(); ++i) {
        if (players[i]->getHandSize() == 0) {
            return i;
        }
    }

    // Если нет - определяем по минимальной сумме
    return determineFishWinner();
}

int DominoGame::determineFishWinner() const {
    int minScore = INT_MAX;
    int winnerIndex = 0;

    for (int i = 0; i < players.size(); ++i) {
        int score = players[i]->getScore();
        if (score < minScore) {
            minScore = score;
            winnerIndex = i;
        }
    }

    return winnerIndex;
}

void DominoGame::saveGame(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not save game to" << filename;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    // Запись версии формата
    const quint32 magic = 0x44F1A9B3;
    out << magic;

    // Основные параметры игры
    out << gameId
        << players.size()
        << currentPlayerIndex
        << currentRound
        << gameEnd;

    // Сохранение базара
    QVector<DominoTile> bazaarTiles = bazaar->getRemainingTiles();
    out << bazaarTiles;

    // Сохранение доски
    out << board;

    // Сохранение игроков
    for (Player* player : players) {
        out << player->getName()
        << player->getHand()
        << player->getScore()
        << player->isBot(); // Сохраняем тип игрока
    }

    file.close();
}

void DominoGame::loadGame(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not load game from" << filename;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    // Проверка версии формата
    quint32 magic;
    in >> magic;
    if (magic != 0x44F1A9B3) {
        qWarning() << "Invalid save file format";
        return;
    }

    // Чтение основных параметров
    in >> gameId
        >> playerCount
        >> currentPlayerIndex
        >> currentRound
        >> gameEnd;

    // Восстановление базара
    QVector<DominoTile> bazaarTiles;
    in >> bazaarTiles;
    bazaar->setTiles(bazaarTiles);

    // Восстановление доски
    in >> board;

    // Восстановление игроков
    players.clear();
    for (int i = 0; i < playerCount; ++i) {
        QString name;
        QVector<DominoTile> hand;
        int score;
        bool isBot;

        in >> name >> hand >> score >> isBot;

        Player* player = isBot ?
                             static_cast<Player*>(new BotPlayer(name)) :
                             new HumanPlayer(name);

        player->setHand(hand);
        player->setScore(score);
        players.append(player);
    }

    file.close();
}

void DominoGame::saveStatistics(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Could not save statistics to" << filename;
        return;
    }

    QTextStream out(&file);
    QDateTime now = QDateTime::currentDateTime();

    out << now.toString(Qt::ISODate) << ";";
    out << playerCount << ";";

    if (gameEnd) {
        int winnerIndex = determineFishWinner();
        out << players[winnerIndex]->getName() << ";";

        for (int i = 0; i < players.size(); ++i) {
            if (i != winnerIndex) {
                out << players[i]->getName() << ":" << players[i]->getScore();
                if (i < players.size() - 1) out << ",";
            }
        }
    }

    out << "\n";
    file.close();
}

void DominoGame::setPlayerCount(int count) {
    playerCount = qBound(2, count, 4);
}

void DominoGame::setStartTileCount(int count) {
    tilesPerPlayer = qBound(5, count, 15);
}

QVector<int> DominoGame::getBoardEnds() const {
    if (board.isEmpty()) {
        return {}; // Если поле пустое, возвращаем пустой вектор
    }
    return {board.first().getLeftValue(), board.last().getRightValue()};
}

Player* DominoGame::getCurrentPlayer() {
    if (currentPlayerIndex >= 0 && currentPlayerIndex < players.size()) {
        return players[currentPlayerIndex];
    }
    return nullptr; // Обработка ошибки при необходимости
}

Player* DominoGame::getNextPlayer() {
    if (NextPlayerIndex >= 0 && NextPlayerIndex  < players.size()) {
        return players[NextPlayerIndex];
    }
    return nullptr; // Обработка ошибки при необходимости
}



void DominoGame::placeTile(DominoTile& tile, bool isLeftEnd) {
    QVector<int> ends = getBoardEnds();

    if (isLeftEnd) {
        // Проверка, совпадает ли правая часть домино с левым концом доски
        if (tile.getRightValue() != ends.first()) {
            tile.flip(); // Переворачиваем домино
        }
        board.prepend(tile);
    } else {
        // Проверка, совпадает ли левая часть домино с правым концом доски
        if (tile.getLeftValue() != ends.last()) {
            tile.flip(); // Переворачиваем домино
        }
        board.append(tile);
    }
    checkForFish();
}

DominoTile DominoGame::getMaxTileWithoutDoubles() const {
    DominoTile maxTile(-1, -1);
    int maxSum = -1;

    for (const auto& player : players) {
        for (const DominoTile& tile : player->getHand()) {
            if (!tile.isDouble() && tile.getTotalValue() > maxSum) {
                maxSum = tile.getTotalValue();
                maxTile = tile;
            }
        }
    }

    return maxTile;
}

int DominoGame::getThirdPlayerIndex() const {
    return (currentPlayerIndex + 2) % players.size();;
}
void DominoGame::updateThirdPlayerIndex() {
    ThirdPlayerIndex = (currentPlayerIndex + 2) % players.size();
}
Player* DominoGame::getThirdPlayer() {
    if (ThirdPlayerIndex >= 0 && ThirdPlayerIndex  < players.size()) {
        return players[ThirdPlayerIndex];
    }
    return nullptr; // Обработка ошибки при необходимости
}

int DominoGame::getFourthPlayerIndex() const {
    return (currentPlayerIndex + 3) % players.size();;
}
void DominoGame::updateFourthPlayerIndex() {
    FourthPlayerIndex = (currentPlayerIndex + 3) % players.size();
}
Player* DominoGame::getFourthPlayer() {
    if (FourthPlayerIndex >= 0 && FourthPlayerIndex  < players.size()) {
        return players[FourthPlayerIndex];
    }
    return nullptr; // Обработка ошибки при необходимости
}

void DominoGame::startNewRound() {
    // Сбросить доску, оставить счет
    board.clear();

    currentRound++;
    startNewGame();

}

bool DominoGame::currentPlayerCanMove() const {
    Player* currentPlayer = players[currentPlayerIndex];
    if(!board.isEmpty())
    {
        int leftEnd = board.first().getLeftValue();
        int rightEnd = board.last().getRightValue();
        return currentPlayer->hasValidMove(leftEnd, rightEnd); // leftEnd/rightEnd - края поля
    }
    return true;
}
