#ifndef CLICKABLE_RECT_H
#define CLICKABLE_RECT_H

#include <QGraphicsRectItem>
#include <QObject>

class ClickableRect : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    using QGraphicsRectItem::QGraphicsRectItem;
    explicit ClickableRect(QGraphicsItem* parent = nullptr);

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};
#endif // CLICKABLE_RECT_H
