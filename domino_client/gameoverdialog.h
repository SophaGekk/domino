#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVector>
#include "player.h"
#include "ui_gameoverdialog.h"

class GameOverDialog : public QDialog {
    Q_OBJECT
public:
    explicit GameOverDialog(const QVector<Player*>& players, QWidget* parent = nullptr);
signals:
    void newRoundRequested();
    void exitToMainMenu();

private:
    QPushButton *newRoundButton;
    QPushButton *exitButton;
    Ui::GameOverDialog ui; // Автоматически сгенерированный UI
};

#endif // GAMEOVERDIALOG_H
