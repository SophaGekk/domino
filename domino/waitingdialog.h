#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H
#include <QDialog>

namespace Ui {
class WaitingDialog;
}

class WaitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog(QWidget *parent = nullptr);
    ~WaitingDialog();

    void setSessionCode(const QString& code);
    void addPlayer(const QString& playerName);
    void setPlayersCount(int current, int required);
    void updatePlayersList(const QStringList& players);

signals:
    void canceled();

private:
    Ui::WaitingDialog *ui;
};
#endif // WAITINGDIALOG_H
