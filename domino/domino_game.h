#ifndef DOMINOGAME_H
#define DOMINOGAME_H
#include <QMainWindow>
#include <QApplication>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>
#include <QObject>
#include <QVector>
#include "domino_tile.h"
class Player;
class DominoTile;
class Bazaar;


class DominoGame : public QObject {
    Q_OBJECT

public:
    DominoGame(int playerCount, int BotCount, const QStringList &playerNames, QObject* parent = nullptr);
    Player* getCurrentPlayer();
    Player* getNextPlayer();

    int getThirdPlayerIndex() const;
    void updateThirdPlayerIndex();
    Player* getThirdPlayer();

    int getFourthPlayerIndex() const;
    void updateFourthPlayerIndex();
    Player* getFourthPlayer();

    void startNewGame(const QStringList &playerNames);

    void saveStatistics(const QString& filename);
    void startNewRound(const QStringList &playerNames);
    // Геттеры для доступа к состоянию игры
    const QVector<Player*>& getPlayers() const;
    const QVector<DominoTile>& getBoard() const;
    QVector<DominoTile>& getBoard();
    void setBoard(const QVector<DominoTile>& newBoard) {
        board = newBoard;
    }

    QVector<int> getBoardEnds() const;

    Bazaar* getBazaar();

    const Bazaar* getBazaarConst() const;
    bool getHasDouble() const {return hasDouble;}
    int getCurrentRound() const { return currentRound; }
    void setCurrentRound(int round) {
        currentRound = round;
    }
    int getCurrentPlayerIndex() const;
    int getNextPlayerIndex() const;
    void updateNextPlayerIndex();
    QVector<int> getScores() const;

    int determineFishWinner() const;
    bool isGameOver() const;

    void setPlayerCount(int count);

    void setStartTileCount(int count);
    DominoTile getMaxTileWithoutDoubles() const;
    void setCurrentPlayerIndex(int index);
    void makeMove() {
        if (isGameOver()) {
            if (doubleCall) return;
            doubleCall = true;
            calculateScores();
            if (gameFinished) {
                emit gameFinish(determineWinner(), getScores()); // Показываем финальное окно
            } else {
                emit RoundEnded(determineWinner(), getScores()); // Показываем окно завершения раунда
            }
            return; // Прекращаем дальнейшие действия
        }
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        emit playerChanged(currentPlayerIndex);
    }
    void calculateScores();
    bool isGameFinished() const {return gameFinished; }

    int getMaxScore() const {return maxScore; }

    void placeTile(DominoTile& tile, bool isLeftEnd);         // Разместить домино
    bool checkForBlockedGame() const;
    int determineWinner() const;
    QString getGameId() const { return gameId; }
    void setGameId(const QString& id) {
        gameId = id;
    }

    bool currentPlayerCanMove() const;
    void checkForFish();
    void determineFirstPlayer();
    bool doubleCall = false;

    QByteArray serializeGameState() const;
    void deserializeGameState(const QByteArray& data);
    void deserializeFromJson(const QJsonObject& state);

signals:
    void gameStarted();
    void playerMoved(int playerIndex, DominoTile til);
    void RoundEnded(int winnerIndex, const QVector<int>& scores);
    void gameFinish(int winnerIndex, const QVector<int>& scores);
    void playerChanged(int currentPlayerIndex);
    void spacePressed();


private:
    void initializePlayers(const QStringList &playerNames);
    void dealTiles();


    int playerCount;
    int BotCount;
    int tilesPerPlayer;
    QVector<Player*> players;
    Bazaar* bazaar;
    QVector<DominoTile> board;
    int currentPlayerIndex;
    int NextPlayerIndex;
    int ThirdPlayerIndex;
    int FourthPlayerIndex;
    bool gameEnd;
    int currentRound = 1;
    bool hasDouble;
    QString gameId; // Формат: "yyyy-MM-ddTHH:mm:ss"
    int maxScore = 100; // Лимит очков по умолчанию 300

    bool gameFinished = false; // Флаг завершения всей игры (огарничение на новые раунды)
};


#endif // DOMINO_GAME_H
