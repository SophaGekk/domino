#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QWidget>
#include <QTime>

class ChatManager : public QObject {
    Q_OBJECT

public:
    explicit ChatManager(QObject* parent = nullptr);
    void sendMessage(const QString& sender, const QString& message);
    QString getChatHistory() const;

signals:
    void newMessage(const QString& formattedMessage);


private:
    QStringList chatHistory;
};


#endif // CHATMANAGER_H
