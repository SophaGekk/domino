#ifndef DOMINOLABEL_H
#define DOMINOLABEL_H

#include <QLabel>
#include <QMouseEvent>


class DominoLabel : public QLabel {
    Q_OBJECT
public:
    explicit DominoLabel(QWidget *parent = nullptr);
    void setDots(int left, int right, bool isVertical, bool m_lineVisible);
    void setHighlighted(bool highlighted);
    void setLineVisible(bool visible);
    void drawDotsHorizontal(QPainter& painter);
    void drawDotsVertical(QPainter& painter);


signals:
    void clicked(); // Сигнал при клике

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawDots(QPainter &painter, int value, const QRect &area);
    void mousePressEvent(QMouseEvent* event) override {
        Q_UNUSED(event);
        emit clicked();
    }

private:
    int leftValue = 0;
    int rightValue = 0;
    bool m_lineVisible = true;
    bool isVertical = true; // Вертикальная ориентация (дубль)
};


#endif // DOMINOLABEL_H
