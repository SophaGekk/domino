#ifndef CLICKABLE_RECT_H
#define CLICKABLE_RECT_H

#include <QGraphicsRectItem>
#include <QObject>

class ClickableRect : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    explicit ClickableRect(QGraphicsItem* parent = nullptr);
    virtual ~ClickableRect() = default;

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};
#endif // CLICKABLE_RECT_H
