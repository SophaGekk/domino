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

    QVector<int> getBoardEnds() const;

    Bazaar* getBazaar();

    const Bazaar* getBazaarConst() const;
    bool getHasDouble() const {return hasDouble;};
    int getCurrentRound() const { return currentRound; };
    int getCurrentPlayerIndex() const;
    int getNextPlayerIndex() const;
    void updateNextPlayerIndex();
    QVector<int> getScores() const;

    int determineFishWinner() const;
    bool isGameOver() const;

    void setPlayerCount(int count);

    void setStartTileCount(int count);
    DominoTile getMaxTileWithoutDoubles() const;

    void makeMove() {
        if (isGameOver()) {
            emit gameEnded(determineWinner(), getScores());
            return; // Прекращаем дальнейшие действия
        }
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        qDebug() << "New player index:" << currentPlayerIndex;
        emit playerChanged(currentPlayerIndex);
    }
    void calculateScores();


    void placeTile(DominoTile& tile, bool isLeftEnd);         // Разместить домино
    bool checkForBlockedGame() const;
    int determineWinner() const;
    QString getGameId() const { return gameId; }
    void setGameId(const QString& id) {
        gameId = id;
    }

    bool currentPlayerCanMove() const;
    void checkForFish();

signals:
    void gameStarted();
    void playerMoved(int playerIndex, DominoTile til);
    void gameEnded(int winnerIndex, const QVector<int>& scores);
    void playerChanged(int currentPlayerIndex);


private:
    void initializePlayers(const QStringList &playerNames);
    void dealTiles();
    void determineFirstPlayer();


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
};


#endif // DOMINO_GAME_H
