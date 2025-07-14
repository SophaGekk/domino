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
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
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

    if (session.gameFinished) return;

    // Проверка, что это ход текущего игрока
    if (session.clientNames[socket] != session.game->getCurrentPlayer()->getName())
        return;

    int left = data["left"].toInt();
    int right = data["right"].toInt();
    bool isLeftEnd = data["is_left_end"].toBool();

    Player* currentPlayer = session.game->getCurrentPlayer();
    DominoTile* tileToRemove = nullptr;

    // Ищем костяшку в руке игрока
    const auto& hand = currentPlayer->getHand();
    for (const DominoTile& tile : hand) {
        if ((tile.getLeftValue() == left && tile.getRightValue() == right) ||
            (tile.getLeftValue() == right && tile.getRightValue() == left)) {
            // Снимаем константность через const_cast
            tileToRemove = const_cast<DominoTile*>(&tile);
            break;
        }
    }
    if (tileToRemove) {
        DominoTile tileCopy = *tileToRemove;
        if (session.game->getBoard().isEmpty()) {// Первый ход
            session.game->getBoard().append(tileCopy);
        } else {
            session.game->placeTile(tileCopy, isLeftEnd);
        }
        qDebug() << "положили";
        currentPlayer->removeTile(*tileToRemove);
        qDebug() << "удалили";
        session.game->makeMove();
        qDebug() << "передали ход";
        broadcastGameState(sessionCode);

        checkGameOver(sessionCode);
    } else {
        qDebug() << "Tile not found in player's hand!";
    }
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
        checkGameOver(sessionCode);
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
        checkGameOver(sessionCode);
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
    chatMessage["time"] = QTime::currentTime().toString("hh:mm");

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

    qDebug() << "Processing create_session for" << playerName;
    if (playersCount < 2 || playersCount > 4)
    {
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
    QJsonObject update;
    update["type"] = "session_created";
    update["code"] = sessionCode;
    update["name"] = playerName;
    update["players"] = static_cast<int>(newSession.clientNames.size());
    update["required"] = newSession.requiredPlayers;

    // Отправляем только хосту
    sendToClient(socket, update);

    qDebug() << "Session created:" << sessionCode << "Host:" << playerName;
}

void Server::processJoinSession(QTcpSocket* socket, const QJsonObject& data)
{

    QString sessionCode = data["code"].toString();
    QString playerName = data["name"].toString();

    qDebug() << "Processing join_session for" << playerName << "code:" << sessionCode;

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

    // Добавляем игрока в сессию
    session.clientNames[socket] = playerName;
    session.connectedNames.insert(playerName);
    socketToSession[socket] = sessionCode;

    // Отправляем подтверждение
    QJsonObject response;
    response["type"] = "joined";
    response["code"] = sessionCode;
    response["name"] = playerName;
    response["players"] = static_cast<int>(session.clientNames.size());
    response["required"] = session.requiredPlayers;
    sendToClient(socket, response);

    // Оповещаем всех о новом игроке
    QJsonObject playerJoined;
    playerJoined["type"] = "player_joined";
    playerJoined["name"] = playerName;
    playerJoined["players"] = static_cast<int>(session.clientNames.size());
    playerJoined["required"] = session.requiredPlayers;

    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, playerJoined);
    }

    QJsonObject update;
    update["type"] = "session_update";
    update["players"] = static_cast<int>(session.clientNames.size());
    update["required"] = session.requiredPlayers;

    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, update);
    }
    qDebug() << "Player joined:" << playerName << "to session:" << sessionCode;

    // Проверяем, можно ли начинать игру
    if (session.clientNames.size() == session.requiredPlayers) {
        startGame(sessionCode);
    }
}

void Server::startGame(const QString& sessionCode)
{
    qDebug() << "Game:" << sessionCode;
    GameSession& session = sessions[sessionCode];

    QStringList playerNames;
    for (QTcpSocket* client : session.clientNames.keys()) {
        playerNames.append(session.clientNames[client]);
    }

    session.game = new DominoGame(session.requiredPlayers, 0, playerNames, this);
    session.gameStarted = true;

    broadcastGameStart(sessionCode);

    // Отправляем начальное состояние игры всем игрокам
    broadcastGameState(sessionCode);

    qDebug() << "Game started for session:" << sessionCode;
}

void Server::broadcastGameStart(const QString& sessionCode)
{
    if (!sessions.contains(sessionCode)) return;

    GameSession& session = sessions[sessionCode];

    QJsonObject startMessage;
    startMessage["type"] = "game_start";

    // Сериализация доски
    QJsonArray boardArray;
    for (const DominoTile& tile : session.game->getBoard()) {
        QJsonObject tileObj;
        tileObj["left"] = tile.getLeftValue();
        tileObj["right"] = tile.getRightValue();
        boardArray.append(tileObj);
    }
    startMessage["board"] = boardArray;

    // Текущий игрок
    startMessage["current_player"] = session.game->getCurrentPlayer()->getName();

    // Счет игроков
    QJsonArray scoresArray;
    for (Player* player : session.game->getPlayers()) {
        scoresArray.append(player->getScore());
    }
    startMessage["player_scores"] = scoresArray;

    // Сериализация базара
    QJsonArray bazaarArray;
    for (const DominoTile& tile : session.game->getBazaar()->getTiles()) {
        QJsonObject tileObj;
        tileObj["left"] = tile.getLeftValue();
        tileObj["right"] = tile.getRightValue();
        bazaarArray.append(tileObj);
    }
    startMessage["bazaar_tiles"] = bazaarArray;

    // Руки игроков
    QJsonArray handsArray;
    for (Player* player : session.game->getPlayers()) {
        QJsonArray handArray;
        for (const DominoTile& tile : player->getHand()) {
            QJsonObject tileObj;
            tileObj["left"] = tile.getLeftValue();
            tileObj["right"] = tile.getRightValue();
            handArray.append(tileObj);
        }
        handsArray.append(handArray);
    }
    startMessage["player_hands"] = handsArray;

    startMessage["current_player"] = session.game->getCurrentPlayer()->getName();

    // Список игроков
    QJsonArray playersArray;
    for (Player* player : session.game->getPlayers()) {
        QJsonObject playerObj;
        playerObj["name"] = player->getName();
        playersArray.append(playerObj);
    }
    startMessage["players"] = playersArray;

    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, startMessage);
    }
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
    for (Player* player : session.game->getPlayers()) {
        scoresArray.append(player->getScore());
    }
    state["player_scores"] = scoresArray;

    // Сериализация базара
    QJsonArray bazaarArray;
    for (const DominoTile& tile : session.game->getBazaar()->getTiles()) {
        QJsonObject tileObj;
        tileObj["left"] = tile.getLeftValue();
        tileObj["right"] = tile.getRightValue();
        bazaarArray.append(tileObj);
    }
    state["bazaar_tiles"] = bazaarArray;

    // Руки игроков
    QJsonArray handsArray;
    for (Player* player : session.game->getPlayers()) {
        QJsonArray handArray;
        for (const DominoTile& tile : player->getHand()) {
            QJsonObject tileObj;
            tileObj["left"] = tile.getLeftValue();
            tileObj["right"] = tile.getRightValue();
            handArray.append(tileObj);
        }
        handsArray.append(handArray);
    }
    state["player_hands"] = handsArray;

    state["current_player"] = session.game->getCurrentPlayer()->getName();

    // Список игроков
    QJsonArray playersArray;
    for (Player* player : session.game->getPlayers()) {
        QJsonObject playerObj;
        playerObj["name"] = player->getName();
        playersArray.append(playerObj);
    }
    state["players"] = playersArray;

    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, state);
    }
}

void Server::slotReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        qDebug() << "Received message:" << str;

        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        if (doc.isNull()) {
            qDebug() << "Invalid JSON received";
            return;
        }

        QJsonObject json = doc.object();
        QString type = json["type"].toString();
        qDebug() << "Message type:" << type;

        if (type == "create_session") {
            processCreateSession(socket, json);
        }

        if (type == "join_session") {
            processJoinSession(socket, json);
        }

        // Для остальных сообщений проверяем привязку к сессии
        if (!socketToSession.contains(socket)) {
            qDebug() << "Socket not in any session, ignoring message";
            return;
        }

        QString sessionCode = socketToSession[socket];

        if (!sessions.contains(sessionCode)) {
            qDebug() << "Session not found for socket";
            return;
        }

        GameSession& session = sessions[sessionCode];

        // Обработка игровых действий (только после начала игры)
        if (session.gameStarted) {
            if (type == "move") {
                processMove(socket, json);
            }
            else if (type == "bazaar") {
                processBazaar(socket);
            }
            else if (type == "skip") {
                processSkip(socket);
            }
            else if (type == "chat") {
                processChat(socket, json);
            }
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

    // Если игра уже начата
    if (session.gameStarted) {
        QJsonObject gameOver;
        gameOver["type"] = "game_terminated";
        gameOver["reason"] = QString("Игра завершена: игрок %1 покинул игру").arg(playerName);

        for (QTcpSocket* client : session.clientNames.keys()) {
            sendToClient(client, gameOver);
            // Отвязываем сокет от сессии перед закрытием
            socketToSession.remove(client);
        }

        // Закрываем соединения с игроками
        closeSessionConnections(session);

        // Удаляем сессию
        removeSession(sessionCode);
        return;
    }

    // Если игра еще не начата
    QJsonObject playerLeft;
    playerLeft["type"] = "player_left";
    playerLeft["name"] = playerName;
    playerLeft["players"] = static_cast<int>(session.clientNames.size());
    playerLeft["required"] = session.requiredPlayers;

    for (QTcpSocket* client : session.clientNames.keys()) {
        sendToClient(client, playerLeft);
    }

    // Удаляем сессию если не осталось игроков
    if (session.clientNames.isEmpty()) {
        removeSession(sessionCode);
    }
}

void Server::closeSessionConnections(GameSession& session)
{
    for (QTcpSocket* client : session.clientNames.keys()) {
        // Отправляем сообщение о закрытии соединения
        QJsonObject closeMsg;
        closeMsg["type"] = "connection_close";
        closeMsg["reason"] = "Игра завершена";
        sendToClient(client, closeMsg);

        // Закрываем сокет
        client->disconnectFromHost();
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

void Server::checkGameOver(const QString& sessionCode) {
    if (!sessions.contains(sessionCode)) return;
    GameSession& session = sessions[sessionCode];

    if (session.game->isGameOver()) {
        int winnerIndex = session.game->determineWinner();
        QString winnerName = (winnerIndex != -1)
                                 ? session.game->getPlayers()[winnerIndex]->getName()
                                 : "";
        bool isDraw = (winnerIndex == -1);

        QJsonObject gameOver;
        gameOver["type"] = "game_over";
        gameOver["winner"] = winnerName;
        gameOver["is_draw"] = isDraw;

        QJsonArray namesArray;
        QJsonArray scoresArray;
        for (Player* player : session.game->getPlayers()) {
            namesArray.append(player->getName());
            scoresArray.append(player->getScore());
        }
        gameOver["player_names"] = namesArray;
        gameOver["player_scores"] = scoresArray;
        gameOver["max_score"] = session.game->getMaxScore();

        for (QTcpSocket* client : session.clientNames.keys()) {
            sendToClient(client, gameOver);
        }
    }
}
