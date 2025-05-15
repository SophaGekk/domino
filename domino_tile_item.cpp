#include "domino_tile_item.h"
#include <QPainter>
#include "MainWindow.h"


DominoTileItem::DominoTileItem(const DominoTile& tile, bool isHandTile, QGraphicsItem* parent)
    : QGraphicsItem(parent), tile(tile), isHandTile(isHandTile), selected(false) {}

QRectF DominoTileItem::boundingRect() const {
    return QRectF(0, 0, 50, 25);
}

void DominoTileItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Рисуем вертикальный прямоугольник
    QRectF rect = boundingRect();
    painter->setBrush(Qt::white);
    painter->drawRoundedRect(rect, 5, 5);

    if (isHandTile) {
        // Координаты для точек (пример для вертикальной костяшки)
        const int dotRadius = 4;
        const QVector<QPointF> dots = calculateDots(tile.getLeftValue(), tile.getRightValue());

        painter->setBrush(Qt::black);
        for (const QPointF& dot : dots) {
            painter->drawEllipse(dot, dotRadius, dotRadius);
        }
    }
}

// Рассчитываем позиции точек для значений
QVector<QPointF> DominoTileItem::calculateDots(int left, int right) {
    QVector<QPointF> dots;
    QRectF rect = boundingRect();
    float w = rect.width() / 3;
    float h = rect.height() / 2;

    // Распределение точек для левой (верхней) части
    addDotsForValue(left, QPointF(w, h/2), dots);
    // Распределение точек для правой (нижней) части
    addDotsForValue(right, QPointF(w, h*1.5), dots);

    return dots;
}

void DominoTileItem::setPossibleMove(bool possible) {
    isPossibleMove = possible;
    update();
}


void DominoTileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    Q_UNUSED(event);
    if (isHandTile) {
        emit tileClicked(data(0).toInt());
    }
}

void DominoTileItem::setSelected(bool selected) {
    this->selected = selected;
    update();
}

DominoTile DominoTileItem::getTile() const {
    return tile;
}

void DominoTileItem::addDotsForValue(int value, const QPointF& offset, QVector<QPointF>& dots) {
    switch (value) {
    case 0:
        // Нет точек
        break;
    case 1:
        dots << offset;
        break;
    case 2:
        dots << offset + QPointF(-5, -5) << offset + QPointF(5, 5);
        break;
    case 3:
        dots << offset << offset + QPointF(-5, -5) << offset + QPointF(5, 5);
        break;
    case 4:
        dots << offset + QPointF(-5, -5) << offset + QPointF(-5, 5)
             << offset + QPointF(5, -5) << offset + QPointF(5, 5);
        break;
    case 5:
        dots << offset << offset + QPointF(-5, -5) << offset + QPointF(5, 5)
             << offset + QPointF(-5, 5) << offset + QPointF(5, -5);
        break;
    case 6:
        dots << offset + QPointF(-5, -5) << offset + QPointF(5, -5)
             << offset + QPointF(-5, 0) << offset + QPointF(5, 0)
             << offset + QPointF(-5, 5) << offset + QPointF(5, 5);
        break;
    default:
        // Можно обработать ошибку или игнорировать
        break;
    }
}
