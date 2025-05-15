#include "dominolabel.h"
#include <QPainter>

DominoLabel::DominoLabel(QWidget *parent) : QLabel(parent) {
    setFixedSize(35, 70); // Вертикальное домино (ширина x высота)
}

void DominoLabel::setDots(int left, int right, bool isVertical, bool lineVisible) {
    leftValue = qBound(0, left, 6);  // Ограничение значений 0-6
    rightValue = qBound(0, right, 6);
    this->isVertical = isVertical;
    m_lineVisible = lineVisible;
    update(); // Триггер перерисовки
}

void DominoLabel::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //qDebug() << "Отрисовка доминошки: " << leftValue << "|" << rightValue;

    if (isVertical) {
        if (m_lineVisible) {
            // Горизонтальная линия
            painter.drawLine(0, height() / 2, width(), height() / 2);
            painter.setPen(QPen(Qt::black, 2)); // Цвет и толщина линии
        }

        // Точки для вертикальных домино (дублей)
        drawDotsVertical(painter);
    } else {
        if (m_lineVisible) {
            // Горизонтальная линия
            painter.drawLine(width() / 2, 0, width() / 2, height());
            painter.setPen(QPen(Qt::black, 2)); // Цвет и толщина линии
        }
        // Точки для горизонтальных домино
        drawDotsHorizontal(painter);
    }

    // Отрисовка точек для левой (верхней) и правой (нижней) частей
//     drawDots(painter, leftValue, QRect(0, 0, width(), height() / 2));
//     drawDots(painter, rightValue, QRect(0, height() / 2, width(), height() / 2));
}

void DominoLabel::drawDotsHorizontal(QPainter& painter) {
    painter.setBrush(Qt::black);
    const int dotRadius = 3;
    const int padding = 8;

    // Левая половина
    drawDots(painter, leftValue, QRect(0, 0, width() / 2, height()));

    // Правая половина
    drawDots(painter, rightValue, QRect(width() / 2, 0, width() / 2, height()));
}

void DominoLabel::drawDotsVertical(QPainter& painter) {
    painter.setBrush(Qt::black);
    const int dotRadius = 3;
    const int padding = 8;

    // Верхняя половина
    drawDots(painter, leftValue, QRect(0, 0, width(), height() / 2));

    // Нижняя половина
    drawDots(painter, rightValue, QRect(0, height() / 2, width(), height() / 2));
}


void DominoLabel::drawDots(QPainter &painter, int value, const QRect &area) {
    painter.setBrush(Qt::black);

    const int dotRadius = 3;
    const int padding = 8;

    // Координаты точек
    QPoint center = area.center();
    QPoint topLeft = area.topLeft() + QPoint(padding, padding);
    QPoint topRight = area.topRight() + QPoint(-padding, padding);
    QPoint bottomLeft = area.bottomLeft() + QPoint(padding, -padding);
    QPoint bottomRight = area.bottomRight() + QPoint(-padding, -padding);
    QPoint midLeft(area.left() + padding, center.y());
    QPoint midRight(area.right() - padding, center.y());

    switch (value) {
    case 1:
        painter.drawEllipse(center, dotRadius, dotRadius);
        break;

    case 2:
        painter.drawEllipse(topLeft, dotRadius, dotRadius);
        painter.drawEllipse(bottomRight, dotRadius, dotRadius);
        break;

    case 3:
        painter.drawEllipse(topLeft, dotRadius, dotRadius);
        painter.drawEllipse(center, dotRadius, dotRadius);
        painter.drawEllipse(bottomRight, dotRadius, dotRadius);
        break;

    case 4:
        painter.drawEllipse(topLeft, dotRadius, dotRadius);
        painter.drawEllipse(topRight, dotRadius, dotRadius);
        painter.drawEllipse(bottomLeft, dotRadius, dotRadius);
        painter.drawEllipse(bottomRight, dotRadius, dotRadius);
        break;

    case 5:
        painter.drawEllipse(topLeft, dotRadius, dotRadius);
        painter.drawEllipse(topRight, dotRadius, dotRadius);
        painter.drawEllipse(center, dotRadius, dotRadius);
        painter.drawEllipse(bottomLeft, dotRadius, dotRadius);
        painter.drawEllipse(bottomRight, dotRadius, dotRadius);
        break;

    case 6:
        painter.drawEllipse(topLeft, dotRadius, dotRadius);
        painter.drawEllipse(topRight, dotRadius, dotRadius);
        painter.drawEllipse(midLeft, dotRadius, dotRadius);
        painter.drawEllipse(midRight, dotRadius, dotRadius);
        painter.drawEllipse(bottomLeft, dotRadius, dotRadius);
        painter.drawEllipse(bottomRight, dotRadius, dotRadius);
        break;

    default:
        break;
    }
}


void DominoLabel::setHighlighted(bool highlighted) {
    setStyleSheet(highlighted
                      ? "background: #e0e0e0; border: 2px solid red;"
                      : "background: white; border: 1px solid black;");
}

void DominoLabel::setLineVisible(bool visible) {
    m_lineVisible = visible;
    //update(); // Перерисовать виджет
}
