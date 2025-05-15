#ifndef DOMINO_TILE_ITEM_H
#define DOMINO_TILE_ITEM_H

#include <QObject>
#include <QGraphicsItem>
#include "domino_tile.h"

class DominoTileItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    DominoTileItem(const DominoTile& tile, bool isHandTile = false, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void setSelected(bool selected);
    DominoTile getTile() const;
    void setPossibleMove(bool possible);
    QVector<QPointF> calculateDots(int left, int right);
    void addDotsForValue(int value, const QPointF& offset, QVector<QPointF>& dots);


signals:
    void tileClicked(int index);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    DominoTile tile;
    bool isHandTile;
    bool selected;

    bool isPossibleMove = false;

};

#endif // DOMINO_TILE_ITEM_H
