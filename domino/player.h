#ifndef PLAYER_H
#define PLAYER_H


#include "domino_game.h"
#include "domino_tile.h"
#include "bazaar.h"

#include <QMainWindow>
#include <QApplication>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>

#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>


class Player {
public:
    explicit Player(const QString& name);

    QString getName() const;
    int getScore() const;
    int getHandSize() const;
    bool hasMatchingTile(int value) const;
    DominoTile getSelectedTile() const;

    virtual std::pair<DominoTile, bool> playTurn(int leftEnd, int rightEnd) = 0;
    void drawTile(Bazaar* bazaar);
    void removeTile(const DominoTile& tile);

    bool hasTile(const DominoTile& tile) const;
    void addTile(const DominoTile& tile) { hand.append(tile); }
    const QVector<DominoTile>& getHand() const;
    void setHand(const QVector<DominoTile>& newHand);
    void setScore(int score);
    virtual bool isBot() const { return false; }

    void setAvatar(const QPixmap& pixmap) { avatar = pixmap; }
    QPixmap getAvatar() const { return avatar; }
    // Метод для получения количества побед
    int getWins() const { return wins; }
    bool hasValidMove(int left, int right) const;
    // Метод для увеличения побед
    void addWin() { wins++; }
    void setWins(int newWins) { wins = newWins; }
protected:
    QString name;
    QVector<DominoTile> hand;
    DominoTile m_selectedTile;
    int score = 0;  // Инициализация поля score
    int wins = 0;
    QPixmap avatar;
};


#endif // PLAYER_H
