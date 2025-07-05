#include "server.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "bazaar.h"
#include "player.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <QTimer>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "Client connected" << socketDescriptor;
}

void Server::sendToClient(QTcpSocket* socket, const QJsonObject& message)
{
    QJsonDocument doc(message);
    QByteArray data = doc.toJson();
    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}

void Server::processMove(QTcpSocket* socket, const QJsonObject& data)
{
    if (!socketToSession.contains(socket)) return;
    QString sessionCode = socketToSession[socket];
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];
    if (!session.gameStarted) return;

    // Проверка, что это ход текущего игрока
    if (session.clientNames[socket] != session.game->getCurrentPlayer()->getName())
        return;

    int left = data["left"].toInt();
    int right = data["right"].toInt();
    bool isLeftEnd = data["is_left_end"].toBool();

    DominoTile tile(left, right);
    session.game->placeTile(tile, isLeftEnd);
    session.game->getCurrentPlayer()->removeTile(tile);
    session.game->makeMove();

    broadcastGameState(sessionCode);
}

void Server::processBazaar(QTcpSocket* socket)
{
    if (!socketToSession.contains(socket)) return;
    QString sessionCode = socketToSession[socket];
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];
    if (!session.gameStarted) return;

    // Проверка, что это ход текущего игрока
    if (session.clientNames[socket] != session.game->getCurrentPlayer()->getName())
        return;

    if (!session.game->getBazaar()->isEmpty()) {
        DominoTile tile = session.game->getBazaar()->drawTile();
        session.game->getCurrentPlayer()->addTile(tile);
        session.game->makeMove();
        broadcastGameState(sessionCode);
    }
}

void Server::processSkip(QTcpSocket* socket)
{
    if (!socketToSession.contains(socket)) return;
    QString sessionCode = socketToSession[socket];
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];
    if (!session.gameStarted) return;

    // Проверка, что это ход текущего игрока
    if (session.game->getCurrentPlayer()->getName() != session.clientNames[socket])
        return;

    if (!session.game->currentPlayerCanMove() && session.game->getBazaar()->isEmpty()) {
        session.game->makeMove();
        broadcastGameState(sessionCode);
    }
}

void Server::processChat(QTcpSocket* socket, const QJsonObject& data)
{
    if (!socketToSession.contains(socket)) return;
    QString sessionCode = socketToSession[socket];
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];
    QString message = data["message"].toString();
    QString sender = session.clientNames[socket];

    // Создаем сообщение для рассылки
    QJsonObject chatMessage;
    chatMessage["type"] = "chat";
    chatMessage["sender"] = sender;
    chatMessage["message"] = message;

    // Рассылаем всем клиентам в сессии
    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, chatMessage);
    }

    qDebug() << "[" << sessionCode << "] Chat message from" << sender << ":" << message;
}

void Server::startServer()
{
    if (this->listen(QHostAddress::Any,2323))
    {
        qDebug()<<"Listening";
    }
    else
    {
        qDebug()<<"Error listening";
    }
}

QString Server::generateSessionCode()
{
    QString code;
    do {
        code = QString::number(QRandomGenerator::global()->bounded(100000, 999999));
    } while (sessions.contains(code));
    return code;
}

void Server::processCreateSession(QTcpSocket* socket, const QJsonObject& data)
{
    QString playerName = data["name"].toString();
    int playersCount = data["players"].toInt();

    if (playersCount < 2 || playersCount > 4) {
        QJsonObject error;
        error["type"] = "error";
        error["message"] = "Invalid players count (2-4)";
        sendToClient(socket, error);
        return;
    }

    QString sessionCode = generateSessionCode();

    // Создаем новую сессию
    GameSession newSession;
    newSession.requiredPlayers = playersCount;
    newSession.hostName = playerName;
    newSession.clientNames[socket] = playerName;
    newSession.connectedNames.insert(playerName);

    // Таймер для очистки неактивных сессий
    newSession.cleanupTimer = new QTimer(this);
    newSession.cleanupTimer->setSingleShot(true);
    newSession.cleanupTimer->setInterval(10 * 60 * 1000); // 10 минут
    connect(newSession.cleanupTimer, &QTimer::timeout, [this, sessionCode]() {
        if (sessions.contains(sessionCode) &&
            sessions[sessionCode].clientNames.size() < sessions[sessionCode].requiredPlayers) {
            removeSession(sessionCode);
        }
    });
    newSession.cleanupTimer->start();

    sessions[sessionCode] = newSession;
    socketToSession[socket] = sessionCode;

    // Отправляем код сессии хосту
    QJsonObject response;
    response["type"] = "session_created";
    response["code"] = sessionCode;
    sendToClient(socket, response);

    qDebug() << "Session created:" << sessionCode << "Host:" << playerName;
}

void Server::processJoinSession(QTcpSocket* socket, const QJsonObject& data)
{
    QString sessionCode = data["code"].toString();
    QString playerName = data["name"].toString();

    if (!sessions.contains(sessionCode)) {
        QJsonObject error;
        error["type"] = "error";
        error["message"] = "Session not found";
        sendToClient(socket, error);
        return;
    }

    GameSession& session = sessions[sessionCode];

    if (session.gameStarted) {
        QJsonObject error;
        error["type"] = "error";
        error["message"] = "Game already started";
        sendToClient(socket, error);
        return;
    }

    if (session.connectedNames.contains(playerName)) {
        QJsonObject error;
        error["type"] = "error";
        error["message"] = "Name already taken";
        sendToClient(socket, error);
        return;
    }

    if (session.clientNames.size() >= session.requiredPlayers) {
        QJsonObject error;
        error["type"] = "error";
        error["message"] = "Session is full";
        sendToClient(socket, error);
        return;
    }

    // Добавляем игрока в сессию
    session.clientNames[socket] = playerName;
    session.connectedNames.insert(playerName);
    socketToSession[socket] = sessionCode;

    // Отправляем подтверждение
    QJsonObject response;
    response["type"] = "joined";
    response["code"] = sessionCode;
    response["players"] = static_cast<int>(session.clientNames.size());
    response["required"] = session.requiredPlayers;
    sendToClient(socket, response);

    // Оповещаем всех о новом игроке
    QJsonObject playerJoined;
    playerJoined["type"] = "player_joined";
    playerJoined["name"] = playerName;
    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, playerJoined);
    }

    // Проверяем, можно ли начинать игру
    if (session.clientNames.size() == session.requiredPlayers) {
        startGame(sessionCode);
    }

    qDebug() << "Player joined:" << playerName << "to session:" << sessionCode;
}

void Server::startGame(const QString& sessionCode)
{
    GameSession& session = sessions[sessionCode];

    QStringList playerNames;
    for (const QString& name : session.clientNames.values()) {
        playerNames.append(name);
    }

    session.game = new DominoGame(session.requiredPlayers, 0, playerNames, this);
    session.gameStarted = true;

    // Отправляем начальное состояние игры всем игрокам
    broadcastGameState(sessionCode);

    qDebug() << "Game started for session:" << sessionCode;
}

void Server::broadcastGameState(const QString& sessionCode)
{
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];
    QJsonObject state;
    state["type"] = "game_state";

    // Сериализация доски
    QJsonArray boardArray;
    for (const DominoTile& tile : session.game->getBoard()) {
        QJsonObject tileObj;
        tileObj["left"] = tile.getLeftValue();
        tileObj["right"] = tile.getRightValue();
        boardArray.append(tileObj);
    }
    state["board"] = boardArray;

    // Текущий игрок
    state["current_player"] = session.game->getCurrentPlayer()->getName();

    // Счет игроков
    QJsonArray scoresArray;
    QVector<int> scores = session.game->getScores();
    for (int score : scores) {
        scoresArray.append(score);
    }
    state["scores"] = scoresArray;

    // Количество костяшек в базаре
    state["bazaar_count"] = session.game->getBazaar()->remainingTilesCount();

    // Отправляем всем клиентам в сессии
    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, state);
    }
}

void Server::slotReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !socketToSession.contains(socket)) return;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject json = doc.object();
        QString type = json["type"].toString();
        QString sessionCode = socketToSession[socket];

        if (!sessions.contains(sessionCode)) return;

        GameSession& session = sessions[sessionCode];

        if (type == "create_session") {
            processCreateSession(socket, json);
        }
        else if (type == "join_session") {
            processJoinSession(socket, json);
        }
        else if (type == "move" && session.gameStarted) {
            processMove(socket, json);
        }
        else if (type == "bazaar" && session.gameStarted) {
            processBazaar(socket);
        }
        else if (type == "skip" && session.gameStarted) {
            processSkip(socket);
        }
        else if (type == "chat") {
            processChat(socket, json);
        }
    }
}

void Server::clientDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !socketToSession.contains(socket)) return;

    QString sessionCode = socketToSession[socket];
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];
    QString playerName = session.clientNames[socket];

    // Удаляем игрока из сессии
    session.clientNames.remove(socket);
    session.connectedNames.remove(playerName);
    socketToSession.remove(socket);

    qDebug() << "Player disconnected:" << playerName << "from session:" << sessionCode;

    // Оповещаем остальных игроков
    QJsonObject playerLeft;
    playerLeft["type"] = "player_left";
    playerLeft["name"] = playerName;
    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, playerLeft);
    }

    // Если игра началась и игроков не осталось - удаляем сессию
    if (session.clientNames.isEmpty()) {
        removeSession(sessionCode);
    }
}

void Server::removeSession(const QString& sessionCode)
{
    if (sessions.contains(sessionCode)) {
        if (sessions[sessionCode].game) {
            delete sessions[sessionCode].game;
        }
        if (sessions[sessionCode].cleanupTimer) {
            sessions[sessionCode].cleanupTimer->stop();
            delete sessions[sessionCode].cleanupTimer;
        }
        sessions.remove(sessionCode);
        qDebug() << "Session removed:" << sessionCode;
    }
}
