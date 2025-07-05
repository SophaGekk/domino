#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QDialog>

namespace Ui {
class NetworkDialog;
}

class NetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkDialog(QWidget *parent = nullptr);
    ~NetworkDialog();

    bool isHosting() const;
    QString getPlayerName() const;
    QString getHostAddress() const;
    quint16 getPort() const;
    int getPlayersCount() const;
    QString getSessionCode() const;
    bool isJoining() const;

private slots:
    void onHostToggled(bool checked);

private:
    Ui::NetworkDialog *ui;
    void updateUi();
};

#endif // NETWORKDIALOG_H
