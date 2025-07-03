#include "networkmanager.h"

void NetworkManager::startHosting(quint16 port) {
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, port)) {
        emit connectionError(tr("Не удалось запустить сервер: ") + server->errorString());
        return;
    }
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::newConnection);
    role = Host;
}

void NetworkManager::connectToHost(const QString& host, quint16 port) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::connected, this, [this, socket]() {
        clients.append(socket);
        connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readData);
        connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::clientDisconnected);
        connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::handleSocketError);
        emit connectedToHost();
    });

    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::handleSocketError);
}

void NetworkManager::sendGameState(const DominoGame& game) {
    if (clients.isEmpty()) return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << GameState << game.serializeGameState();

    for (QTcpSocket* client : clients) {
        client->write(data);
    }
}

void NetworkManager::sendPlayerMove(const DominoTile& tile, bool isLeftEnd) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << PlayerMove << tile.getLeftValue() << tile.getRightValue() << isLeftEnd;

    if (role == Host) {
        for (QTcpSocket* client : clients) {
            client->write(data);
        }
    } else if (hostSocket) {
        hostSocket->write(data);
    }
}


void NetworkManager::processData(const QByteArray& data) {
    QDataStream stream(data);
    quint8 messageType;
    stream >> messageType;

    switch (static_cast<MessageType>(messageType)) {
    case GameState: {
        emit gameStateReceived(data.mid(sizeof(messageType)));
        break;
    }
    case PlayerMove: {
        int left, right;
        bool isLeftEnd;
        stream >> left >> right >> isLeftEnd;
        emit playerMoveReceived(DominoTile(left, right), isLeftEnd);
        break;
    }
        // ... обработка других типов сообщений
    }
}
