// clickable_rect.cpp
#include "clickable_rect.h"

ClickableRect::ClickableRect(QGraphicsItem* parent)
    : QObject(), QGraphicsRectItem(parent) {}

void ClickableRect::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    Q_UNUSED(event);
    emit clicked();
}
