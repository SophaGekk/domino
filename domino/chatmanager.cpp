#include "chatmanager.h"

ChatManager::ChatManager(QObject* parent) : QObject(parent) {}

void ChatManager::sendMessage(const QString& sender, const QString& message) {
    QString formattedMessage = QString("[%1] <b>%2:</b> %3")
    .arg(QTime::currentTime().toString("hh:mm"), sender, message);

    chatHistory.append(formattedMessage);
    emit newMessage(formattedMessage); // Измените сигнал для передачи форматированного сообщения
}


// void ChatManager::sendMessage(/*...*/) {
//     if (chatHistory.size() > 100) chatHistory.removeFirst();
//     // ... остальной код ...
// }
