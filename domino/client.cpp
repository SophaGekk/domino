#include "client.h"
#include "gamewindow.h"

Client::Client(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)), m_playerName("")
{
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

Client::~Client()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
    }
}
void Client::disconnectFromHost()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
}

void Client::connectToServer(const QString& host, quint16 port, const QString& playerName)
{
    qDebug() << "Connecting to server:" << host << ":" << port;
    m_playerName = playerName; // Сохраняем имя
    socket->connectToHost(host, port);

    // Добавляем таймаут подключения
    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed:" << socket->errorString();
        emit errorOccurred("Connection timeout");
    }
}

void Client::onReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    while (socket->bytesAvailable() > 0) {
        if (in.status() != QDataStream::Ok) {
            qDebug() << "Stream error";
            break;
        }
        in.startTransaction();

        QString str;
        in >> str;

        if (!in.commitTransaction()) {
            // Не хватает данных, ждём больше
            break;
        }

        qDebug() << "Received message:" << str;
        processMessage(str);
    }
}

void Client::processMessage(const QString& message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull()) {
        qDebug() << "Invalid JSON received";
        return;
    }

    QJsonObject json = doc.object();
    QString type = json["type"].toString();
    qDebug() << "Processing message type:" << type;

    if (type == "session_update") {
        int players = json["players"].toInt();
        int required = json["required"].toInt();
        qDebug() << "Session update:" << players << "/" << required;
        emit sessionUpdated(players, required);
    }
    else if (type == "session_created") {
        processSessionCreated(json);
    }
    else if (type == "joined" || type == "player_joined") {
        processPlayerJoined(json);
    }
    else if (type == "error") {
        processError(json);
    }
    else if (type == "game_state") {
        qDebug() << "Game state received";
        emit gameStarted(json);
    }
    else if (type == "chat") {
        QString sender = json["sender"].toString();
        QString message = json["message"].toString();
        emit newChatMessage(sender, message);
    }
}

void Client::sendMove(const DominoTile& tile, bool isLeftEnd)
{
    QJsonObject message;
    message["type"] = "move";
    message["left"] = tile.getLeftValue();
    message["right"] = tile.getRightValue();
    message["is_left_end"] = isLeftEnd;

    QJsonDocument doc(message);
    QByteArray data = doc.toJson();
    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}

void Client::sendBazaarRequest()
{
    QJsonObject message;
    message["type"] = "bazaar";

    QJsonDocument doc(message);
    QByteArray data = doc.toJson();

    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}

void Client::sendSkipRequest()
{
    QJsonObject message;
    message["type"] = "skip";

    QJsonDocument doc(message);
    QByteArray data = doc.toJson();

    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}

void Client::sendChatMessage(const QString& message)
{
    QJsonObject json;
    json["type"] = "chat";
    json["message"] = message;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}


void Client::createSession(const QString& playerName, int playersCount)
{
    QJsonObject request;
    request["type"] = "create_session";
    request["name"] = playerName;
    request["players"] = playersCount;

    QJsonDocument doc(request);
    QByteArray data = doc.toJson();

    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}

void Client::joinSession(const QString& sessionCode, const QString& playerName)
{
    QJsonObject request;
    request["type"] = "join_session";
    request["code"] = sessionCode;
    request["name"] = playerName;

    QJsonDocument doc(request);
    QByteArray data = doc.toJson();

    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_6_2);
    out << QString::fromUtf8(data);
}

void Client::processSessionCreated(const QJsonObject& json)
{
    QString code = json["code"].toString();
    QString name = json["name"].toString();
    int current = json["players"].toInt();
    int required = json["required"].toInt();
    emit sessionCreated(code, name, current, required);
}

void Client::processPlayerJoined(const QJsonObject& json)
{
    QString name = json["name"].toString();
    int current = json["players"].toInt();
    int required = json["required"].toInt();
    emit playerJoined(name, current, required);
}

void Client::processError(const QJsonObject& json)
{
    QString message = json["message"].toString();
    emit errorOccurred(message);
}

