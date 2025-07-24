#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include "domino_tile.h"
#include <QMainWindow>

class GameWindow;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QMainWindow* mainWindow, QObject *parent = nullptr);
    ~Client();
    void sendNewRoundRequest();
    void sendMove(const DominoTile& tile, bool isLeftEnd);
    void sendBazaarRequest();
    void sendSkipRequest();
    void sendChatMessage(const QString& message);
    void setGameWindow(GameWindow* window) { gameWindow = window; }
    void connectToServer(const QString& host, quint16 port, const QString& playerName);
    void createSession(const QString& playerName, int playersCount);
    void joinSession(const QString& sessionCode, const QString& playerName);
    void disconnectFromHost();
    QString getPlayerName() const { return m_playerName; }
    void sendBazaarTileRequest(const DominoTile& tile);
    void leaveSession();
    void disconnectFromServer();

signals:
    void newChatMessage(const QString& sender, const QString& message);
    void sessionCreated(const QString& sessionCode, const QString& playerName, int players, int required);
    void playerJoined(const QString& playerName, int currentPlayers, int requiredPlayers);
    void playerLeft(const QString& playerName, int currentPlayers, int requiredPlayers);
    void errorOccurred(const QString& message);
    void gameStarted(const QJsonObject& state);
    void gameStateReceived(const QJsonObject& state);
    void sessionUpdated(int players, int required);
    void returnToMainMenuRequested();
    void gameOver(const QVector<QString>& playerNames, const QVector<int>& playerScores, int maxScore, const QString& winner, bool isDraw);
    void roundOver(const QVector<QString>& playerNames, const QVector<int>& playerScores, int maxScore, const QString& winner, bool isDraw);

private slots:
    void onReadyRead();

private:
    void processSessionCreated(const QJsonObject& json);
    void processPlayerJoined(const QJsonObject& json);

    void processPlayerLeft(const QJsonObject& json);

    void processError(const QJsonObject& json);
    void processMessage(const QString& str);


    QTcpSocket *socket;
    QByteArray Data;
    QString m_playerName;
    GameWindow* gameWindow = nullptr;
    QMainWindow* m_mainWindow;
};

#endif // CLIENT_H
