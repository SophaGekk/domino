#ifndef GAMEMODEDIALOG_H
#define GAMEMODEDIALOG_H

#include <QDialog>

namespace Ui {
class GameModeDialog;
}

class GameModeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameModeDialog(QWidget *parent = nullptr);
    ~GameModeDialog();

    bool isNetworkGameSelected() const;

private:
    Ui::GameModeDialog *ui;
};

#endif // GAMEMODEDIALOG_H
