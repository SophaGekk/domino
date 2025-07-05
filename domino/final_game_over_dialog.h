#ifndef FINAL_GAME_OVER_DIALOG_H
#define FINAL_GAME_OVER_DIALOG_H

#include <QDialog>
#include <QVector>
#include "player.h"

class FinalGameOverDialog : public QDialog {
    Q_OBJECT
public:
    explicit FinalGameOverDialog(
        const QVector<Player*>& players,
        const QVector<int>& totalScores,
        int maxScore,
        bool isDraw,
        QWidget* parent = nullptr
        );

signals:
    void exitToMainMenu();

private:
    void setupUI();

    QVector<Player*> players;
    QVector<int> totalScores;
    int maxScore;
    bool isDraw;
};
#endif // FINAL_GAME_OVER_DIALOG_H
