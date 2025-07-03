#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include "domino_game.h"
#include "domino_tile.h"

class NetworkManager : public QObject {
    Q_OBJECT

public:
    enum Role { Host, Client };
    enum MessageType {
        GameState,
        PlayerMove,
        ChatMessage,
        PlayerJoined,
        GameOver
    };

    explicit NetworkManager(QObject* parent = nullptr);

    void startHosting(quint16 port);
    void connectToHost(const QString& host, quint16 port);
    void sendGameState(const DominoGame& game);
    void sendPlayerMove(const DominoTile& tile, bool isLeftEnd);
    void sendChatMessage(const QString& sender, const QString& message);
    void sendGameOver(int winnerIndex, const QVector<int>& scores);

    Role getRole() const { return role; }

signals:
    void gameStateReceived(const QByteArray& stateData);
    void playerMoveReceived(const DominoTile& tile, bool isLeftEnd);
    void chatMessageReceived(const QString& sender, const QString& message);
    void gameOverReceived(int winnerIndex, const QVector<int>& scores);
    void connectedToHost();
    void connectionError(const QString& error);
    void newPlayerConnected(const QString& playerName);

private slots:
    void newConnection();
    void readData();
    void clientDisconnected();
    void handleSocketError(QAbstractSocket::SocketError error);

private:
    void processData(const QByteArray& data);

    QTcpServer* server;
    QTcpSocket* hostSocket = nullptr;
    QList<QTcpSocket*> clients;
    Role role = Client;
    QDataStream stream;
};

#endif // NETWORKMANAGER_H
