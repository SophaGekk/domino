#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include "domino_tile.h"

class GameWindow;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void sendMove(const DominoTile& tile, bool isLeftEnd);
    void sendBazaarRequest();
    void sendSkipRequest();
    void sendChatMessage(const QString& message);
    void setGameWindow(GameWindow* window) { gameWindow = window; }
    void connectToServer(const QString& host, quint16 port, const QString& playerName);
    void createSession(const QString& playerName, int playersCount);
    void joinSession(const QString& sessionCode, const QString& playerName);
    void disconnectFromHost();

signals:
    void newChatMessage(const QString& sender, const QString& message);
    void gameStateReceived(const QJsonObject& state);
    void sessionCreated(const QString& sessionCode);
    void joined(int currentPlayers, int requiredPlayers);
    void playerJoined(const QString& playerName);
    void errorOccurred(const QString& message);
    void gameStarted(const QJsonObject& state);

private slots:
    void onReadyRead();

private:
    void processSessionCreated(const QJsonObject& json);
    void processJoined(const QJsonObject& json);
    void processPlayerJoined(const QJsonObject& json);
    void processError(const QJsonObject& json);

    QTcpSocket *socket;
    QByteArray Data;
    QString m_playerName;
    GameWindow* gameWindow = nullptr;
};

#endif // CLIENT_H
