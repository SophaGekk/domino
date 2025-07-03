#ifndef NETWORKGAMEWINDOW_H
#define NETWORKGAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>

class NetworkGameWindow : public QWidget {
    Q_OBJECT

public:
    explicit NetworkGameWindow(QWidget* parent = nullptr);

signals:
    void hostGameRequested(quint16 port);
    void joinGameRequested(const QString& address, quint16 port);
    void backToMainMenu();

private:
    QLineEdit* ipAddressInput;
    QLineEdit* portInput;
    QPushButton* hostButton;
    QPushButton* joinButton;
    QPushButton* backButton;
};

#endif // NETWORKGAMEWINDOW_H
