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
#include <QJsonArray>
#include <QJsonValue>

// Конструктор
DominoGame::DominoGame(int playerCount, int BotCount, const QStringList &playerNames, QObject* parent)
    : QObject(parent), playerCount(playerCount), BotCount(BotCount), currentPlayerIndex(0), gameEnd(false) {
    bazaar = new Bazaar();
    initializePlayers(playerNames);
    startNewGame(playerNames);

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
void DominoGame::setCurrentPlayerIndex(int index) {
    currentPlayerIndex = index;
}
int DominoGame::getNextPlayerIndex() const {
    return (currentPlayerIndex + 1) % players.size();;
}

// Инициализация игроков
void DominoGame::initializePlayers(const QStringList &playerNames) {
    players.clear();

    // Человеческие игроки
    for (const QString &name : playerNames) {
        players.append(new HumanPlayer(name));
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
        qDebug() << "Player" << player->getName() << "hand size:" << player->getHandSize();
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

void DominoGame::startNewGame(const QStringList &playerNames) {
    setGameId(QDateTime::currentDateTime().toString(Qt::ISODate));
    // Очищаем предыдущее состояние
    board.clear();
    bazaar->initialize();

    // Инициализируем игроков
    initializePlayers(playerNames);

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
    emit RoundEnded(winnerIndex, getScores());
}

void DominoGame::calculateScores() {
    bool scoreLimitReached = false;

    // Определяем победителя и тип окончания игры
    int winnerIndex = determineWinner();
    int winnerScore = 0;

    // Рассчитываем очки для всех игроков
    int totalScore = 0;
    for (Player* player : players) {
        int score = 0;
        for (const DominoTile& tile : player->getHand()) {
            score += tile.getTotalValue();
        }
        if(player == players[winnerIndex]){winnerScore = score;}
        totalScore += score;
    }

    // Начисляем очки победителю по правилам
    if (winnerIndex != -1) {
        Player* winner = players[winnerIndex];
        winner->setScore(totalScore - winnerScore);
    }

    // Проверяем достижение лимита очков
    for (Player* player : players) {
        if (player->getScore() >= maxScore) {
            scoreLimitReached = true;
        }
    }
    gameFinished = scoreLimitReached;
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
    int winnerIndex = -1;
    bool hasTie = false;

    for (int i = 0; i < players.size(); ++i) {
        int score = players[i]->getScore();
        if (score < minScore) {
            minScore = score;
            winnerIndex = i;
            hasTie = false;  // Новый минимум - сбрасываем флаг ничьи
        } else if (score == minScore) {
            hasTie = true;   // Обнаружен дубликат минимального счета
        }
    }

    // Если есть ничья или игроков нет, возвращаем -1
    if (hasTie || winnerIndex == -1) {
        return -1;
    }
    return winnerIndex;
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
    return nullptr; // Обработка ошибки
}

Player* DominoGame::getNextPlayer() {
    if (NextPlayerIndex >= 0 && NextPlayerIndex  < players.size()) {
        return players[NextPlayerIndex];
    }
    return nullptr; // Обработка ошибки
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

void DominoGame::startNewRound(const QStringList& playerNames) {
    // Сохраняем статистику игроков
    QVector<int> scores;
    QVector<int> wins;
    for (Player* player : players) {
        scores.append(player->getScore());
        wins.append(player->getWins());
    }

    // Полная переинициализация игры
    initializePlayers(playerNames);

    // Восстанавливаем счет
    for (int i = 0; i < players.size(); ++i) {
        if (i < scores.size()) {
            players[i]->setScore(scores[i]);
        }
        if (i < wins.size()) {
            players[i]->setWins(wins[i]); // Восстанавливаем победы
        }
    }

    // Сброс состояния
    board.clear();
    bazaar->clear();
    currentPlayerIndex = 0;
    currentRound++;
    gameEnd = false;

    // Новая раздача
    dealTiles();
    determineFirstPlayer();
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

QByteArray DominoGame::serializeGameState() const {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << gameId;
    stream << currentPlayerIndex;
    stream << currentRound;

    // Сериализация доски
    stream << board.size();
    for (const DominoTile& tile : board) {
        stream << tile.getLeftValue() << tile.getRightValue();
    }

    // Сериализация игроков
    stream << players.size();
    for (const Player* player : players) {
        stream << player->getName();
        stream << player->getHand().size();
        for (const DominoTile& tile : player->getHand()) {
            stream << tile.getLeftValue() << tile.getRightValue();
        }
    }

    return data;
}

void DominoGame::deserializeGameState(const QByteArray& data) {
    QDataStream stream(data);

    stream >> gameId;
    stream >> currentPlayerIndex;
    stream >> currentRound;

    // Десериализация доски
    int boardSize;
    stream >> boardSize;
    board.clear();
    for (int i = 0; i < boardSize; i++) {
        int left, right;
        stream >> left >> right;
        board.append(DominoTile(left, right));
    }

    // Десериализация игроков
    int playerCount;
    stream >> playerCount;
    players.clear();
    for (int i = 0; i < playerCount; i++) {
        QString name;
        stream >> name;

        Player* player = new HumanPlayer(name); // Или бот в зависимости от типа
        int handSize;
        stream >> handSize;

        QVector<DominoTile> hand;
        for (int j = 0; j < handSize; j++) {
            int left, right;
            stream >> left >> right;
            hand.append(DominoTile(left, right));
        }
        player->setHand(hand);
        players.append(player);
    }
}

void DominoGame::deserializeFromJson(const QJsonObject& state)
{
    // Очищаем текущее состояние
    board.clear();              // Очищаем доску
    bazaar->clear();       // Очищаем базар
    currentPlayerIndex = 0;

    // Очищаем руки игроков и сбрасываем их очки
    for (Player* player : players) {
        player->clearHand();
        player->clearScore();
    }

    // Восстанавливаем доску
    QJsonArray boardArray = state["board"].toArray();
    for (const QJsonValue& tileValue : boardArray) {
        QJsonObject tileObj = tileValue.toObject();
        DominoTile tile(tileObj["left"].toInt(), tileObj["right"].toInt());
        board.append(tile);
    }

    // Восстанавливаем очки игроков
    if (state.contains("player_scores")) {
        QJsonArray scoresArray = state["player_scores"].toArray();
        for (int i = 0; i < scoresArray.size() && i < players.size(); ++i) {
            players[i]->setScore(scoresArray[i].toInt());
        }
    }

    // Восстанавливаем базар
    QJsonArray bazaarArray = state["bazaar_tiles"].toArray();
    QVector<DominoTile> bazaarTiles;
    for (const QJsonValue& tileValue : bazaarArray) {
        QJsonObject tileObj = tileValue.toObject();
        bazaarTiles.append(DominoTile(tileObj["left"].toInt(), tileObj["right"].toInt()));
    }
    bazaar->setTiles(bazaarTiles);

    // Восстанавливаем текущего игрока
    QString currentPlayerName = state["current_player"].toString();
    for (int i = 0; i < players.size(); ++i) {
        if (players[i]->getName() == currentPlayerName) {
            currentPlayerIndex = i;
            break;
        }
    }

    // Восстанавливаем руки игроков
    if (state.contains("player_hands")) {
        QJsonArray handsArray = state["player_hands"].toArray();
        for (int i = 0; i < handsArray.size() && i < players.size(); ++i) {
            QJsonArray handArray = handsArray[i].toArray();
            QVector<DominoTile> hand;
            for (const QJsonValue& tileValue : handArray) {
                QJsonObject tileObj = tileValue.toObject();
                hand.append(DominoTile(tileObj["left"].toInt(), tileObj["right"].toInt()));
            }
            players[i]->setHand(hand);
        }
    }

    // Помечаем игру как начатую
    emit gameStarted();
}
