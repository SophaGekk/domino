#ifndef BAZAAR_H
#define BAZAAR_H
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

class DominoTile;



class Bazaar {
public:
    Bazaar();

    void initialize();
    void clear() { tiles.clear(); }
    bool isEmpty() const;
    DominoTile drawTile();
    int remainingTilesCount() const;
    const QVector<DominoTile>& getRemainingTiles() const;
    void setTiles(const QVector<DominoTile>& newTiles);
    const QVector<DominoTile>& getTiles() const;
    void removeTile(const DominoTile& tile);

private:
    QVector<DominoTile> tiles;
};


#endif // BAZAAR_H
