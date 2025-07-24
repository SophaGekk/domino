#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QMap>
#include <QJsonObject>
#include "domino_game.h"

struct GameSession {
    DominoGame* game = nullptr;
    bool gameStarted = false;
    bool gameFinished = false;
    int requiredPlayers;
    QMap<QTcpSocket*, QString> clientNames; // socket -> player name
    QSet<QString> connectedNames; // для проверки уникальности имен
    QString hostName;
    QTimer* cleanupTimer; // для удаления неактивных сессий
    QTcpSocket* host = nullptr;
};


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    void sendToClient(QTcpSocket* socket, const QJsonObject& message);
    void startServer();
    void checkGameOver(const QString& sessionCode);


private:
    QVector<QTcpSocket*> Sockets;
    QByteArray Data;
    QMap<QString, GameSession> sessions; // session code -> GameSession
    QMap<QTcpSocket*, QString> socketToSession; // socket -> session code'

    void broadcastGameState(const QString& sessionCode);
    void broadcastGameStart(const QString& sessionCode);
    void processCreateSession(QTcpSocket* socket, const QJsonObject& data);
    void processJoinSession(QTcpSocket* socket, const QJsonObject& data);
    void processMove(QTcpSocket* socket, const QJsonObject& data);
    void processBazaar(QTcpSocket* socket);
    void processSkip(QTcpSocket* socket);
    void processChat(QTcpSocket* socket, const QJsonObject& data);
    void startGame(const QString& sessionCode);
    void removeSession(const QString& sessionCode);
    QString generateSessionCode();
    void closeSessionConnections(GameSession& session);
    void processBazaarTile(QTcpSocket* socket, const QJsonObject& data);
    void sendErrorMessage(QTcpSocket* socket, const QString& message);
    void processNewRound(QTcpSocket* socket);
    void clientLeft();



public slots:
    void incomingConnection(qintptr socketDescriptor) override;
    void slotReadyRead();
    void clientDisconnected();
};

#endif // SERVER_H
